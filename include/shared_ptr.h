#ifndef _SHARED_PTR_
#define _SHARED_PTR_


#include <stddef.h>
#include "allocator.h"
#include "type_traits.h"
#include "utility.h"
#include "unique_ptr.h"


namespace HxSTL {

    class ref_count_base {
    private:
        virtual void destroy_object() = 0;
        virtual void destory_this() = 0;
    private:
        long _use;
        long _weak;
    protected:
        // 对象本身视为 weak reference
        ref_count_base(): _use(1), _weak(1) {}
    public:
        virtual ~ref_count_base() {}

        virtual void* get_deleter() { return static_cast<void*>(0); }

        void inc_ref() { ++_use; }

        void inc_wref() { ++_weak; }

        void dec_ref() {
            if (--_use == 0) {
                destroy_object();
                dec_wref();
            }
        }

        void dec_wref() {
            if (--_weak == 0) {
                destory_this();
            }
        }

        long use_count() const { return _use; }
    };

    template <class T>
    class ref_count: public ref_count_base {
    private:
        void destroy_object() { if (_ptr) delete _ptr; }

        void destory_this() { delete this; }
    protected:
        T* _ptr;
    public:
        ref_count(T* p): _ptr(p) {}
    };

    template <class T, class Deleter>
    class ref_count_del: public ref_count_base {
    private:
        void destroy_object() {
            if (_del) {
                _del(_ptr);
            } else if (_ptr) {
                delete _ptr;
            }
        }

        void destory_this() { delete this; }
    protected:
        T* _ptr;
        Deleter _del;
    public:
        ref_count_del(T* p, Deleter d): _ptr(p), _del(d) {}

        void* get_deleter() { return &_del; }
    };

    template <class T, class Deleter, class Alloc>
    class ref_count_del_alloc: public ref_count_base {
    private:
        void destroy_object() {
            if (_del) {
                _del(_ptr);
            } else if (_ptr) {
                delete _ptr;
            }
        }

        void destory_this() {
            typename Alloc::template rebind<ref_count_del_alloc<T, Deleter, Alloc>>::other bind_alloc(_alloc);
            bind_alloc.destroy(this);
            bind_alloc.deallocate(this, 1);
        }
    protected:
        T* _ptr;
        Deleter _del;
        Alloc _alloc;
    public:
        ref_count_del_alloc(T* p, Deleter d, const Alloc& alloc): _ptr(p), _del(d), _alloc(alloc) {}

        void* get_deleter() { return &_del; }
    };

    template <class T, class Alloc>
    class ref_count_alloc_obj: public ref_count_base {
    private:
        void destroy_object() {
            _alloc.destroy(_ptr);
            _alloc.deallocate(_ptr, 1);
        }

        void destory_this() {
            typename Alloc::template rebind<ref_count_alloc_obj<T, Alloc>>::other bind_alloc(_alloc);
            bind_alloc.destroy(this);
            bind_alloc.deallocate(this, 1);
        }
    protected:
        T* _ptr;
        Alloc _alloc;
    public:
        ref_count_alloc_obj(T* p, const Alloc& alloc): _ptr(p), _alloc(alloc) {}
    };

    template <class T>
    class weak_ptr;

    template <class T>
    class shared_ptr {
    public:
        typedef T           element_type;
    protected:
        element_type* _ptr;
        ref_count_base* _cnt;
    public:
        shared_ptr(): _ptr(nullptr), _cnt(nullptr) {}

        shared_ptr(nullptr_t): _ptr(nullptr), _cnt(nullptr) {}

        template <class Y>
        shared_ptr(Y* p): _ptr(p), _cnt(new ref_count<T>(_ptr)) {}

        template <class Y, class Deleter>
        shared_ptr(Y* p, Deleter d): _ptr(p), _cnt(new ref_count_del<T, Deleter>(_ptr, d)) {}

        template <class Deleter>
        shared_ptr(nullptr_t p, Deleter d): _ptr(p), _cnt(new ref_count_del<T, Deleter>(_ptr, d)) {}

        template <class Y, class Deleter, class Alloc>
        shared_ptr(Y* p, Deleter d, Alloc alloc): _ptr(p) {
            typename Alloc::template rebind<ref_count_del_alloc<T, Deleter, Alloc>>::other bind_alloc(alloc);
            _cnt = bind_alloc.allocate(1);
            // 如果最后一个参数传 bind_alloc 为什么 destroy_this 还是要 rebind ?
            bind_alloc.construct(static_cast<ref_count_del_alloc<T, Deleter, Alloc>*>(_cnt), _ptr, d, alloc);
        }

        template <class Deleter, class Alloc>
        shared_ptr(nullptr_t p, Deleter d, Alloc alloc): _ptr(p) {
            typename Alloc::template rebind<ref_count_del_alloc<T, Deleter, Alloc>>::other bind_alloc(alloc);
            _cnt = bind_alloc.allocate(1);
            bind_alloc.construct(static_cast<ref_count_del_alloc<T, Deleter, Alloc>*>(_cnt), _ptr, d, alloc);
        }

        template <class Y>
        shared_ptr(const shared_ptr<Y>& r, element_type* p): _ptr(p), _cnt(r._cnt) { _cnt -> inc_ref(); }

        shared_ptr(const shared_ptr& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_ref(); }

        template <class Y>
        shared_ptr(const shared_ptr<Y>& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_ref(); }

        shared_ptr(shared_ptr&& r): _ptr(r._ptr), _cnt(r._cnt) {
            r._ptr = nullptr;
            r._cnt = nullptr;
        }

        template <class Y>
        shared_ptr(shared_ptr<Y>&& r): _ptr(r._ptr), _cnt(r._cnt) {
            r._ptr = nullptr;
            r._cnt = nullptr;
        }

        template <class Y>
        explicit shared_ptr(const weak_ptr<Y>& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_ref(); }

        template <class Y, class Deleter>
        shared_ptr(unique_ptr<Y, Deleter>&& r)
        : _ptr(r.release()), _cnt(new ref_count_del<T, Deleter>(_ptr, r.get_deleter())) {}

        ~shared_ptr() { if (_cnt) _cnt -> dec_ref(); }

        shared_ptr& operator=(const shared_ptr& r) {
            shared_ptr(r).swap(*this);
            return *this;
        }

        template <class Y>
        shared_ptr& operator=(const shared_ptr<Y>& r) {
            shared_ptr<T>(r).swap(*this);
            return *this;
        }

        shared_ptr& operator=(shared_ptr&& r) {
            shared_ptr<T>(HxSTL::move(r)).swap(*this);
            return *this;
        }

        template <class Y>
        shared_ptr& operator=(shared_ptr<Y>&& r) {
            shared_ptr<T>(HxSTL::move(r)).swap(*this);
            return *this;
        }

        template <class Y, class Deleter>
        shared_ptr& operator=(unique_ptr<Y, Deleter>&& r) {
            shared_ptr<T>(HxSTL::move(r)).swap(*this);
            return *this;
        }

        void reset() { shared_ptr<T>().swap(*this); }

        template <class Y>
        void reset(Y* p) { shared_ptr<T>(p).swap(*this); }

        template <class Y, class Deleter>
        void reset(Y* p, Deleter d) { shared_ptr<T>(p, d).swap(*this); }

        template <class Y, class Deleter, class Alloc>
        void reset(Y* p, Deleter d, Alloc alloc) { shared_ptr<T>(p, d, alloc).swap(*this); }

        void swap(shared_ptr& r) {
            HxSTL::swap(_ptr, r._ptr);
            HxSTL::swap(_cnt, r._cnt);
        }

        element_type* get() const { return _ptr; }

        element_type& operator*() const { return *get(); }

        element_type* operator->() const { return get(); }

        element_type& operator[](ptrdiff_t idx) const { return get()[idx]; }

        long use_count() const { return _cnt ? _cnt -> use_count() : 0; }

        bool unique() const { return use_count() == 1; }

        explicit operator bool() const { return get() != nullptr; }

        template <class Y>
        bool owner_before(const shared_ptr<Y>& other) const { return _cnt < other._cnt; }

        template <class Y>
        bool owner_before(const weak_ptr<Y>& other) const { return _cnt < other._cnt; }
    private:
        template <class Alloc>
        shared_ptr(T* p, const Alloc& alloc, allocator_arg_t): _ptr(p) {
            typename Alloc::template rebind<ref_count_alloc_obj<T, Alloc>>::other bind_alloc(alloc);
            _cnt = bind_alloc.allocate(1);
            bind_alloc.construct(static_cast<ref_count_alloc_obj<T, Alloc>*>(_cnt), _ptr, alloc);
        }
    public:
        template <class Deleter, class Y>
        friend Deleter* get_deleter(const shared_ptr<Y>& p);
        template <class Y, class Alloc, class... Args>
        friend shared_ptr<Y> allocate_shared(const Alloc& alloc, Args&&... args); 
        template <class Y>
        friend class shared_ptr;
        template <class Y>
        friend class weak_ptr;
    };

    template <class T, class... Args>
    shared_ptr<T> make_shared(Args&&... args) {
        return shared_ptr<T>(new T(args...));
    }

    template <class T, class Alloc, class... Args>
    shared_ptr<T> allocate_shared(const Alloc& alloc, Args&&... args) {
        typename Alloc::template rebind<T>::other bind_alloc(alloc);
        T* p = bind_alloc.allocate(1);
        bind_alloc.construct(p, args...);
        return shared_ptr<T>(p, alloc, allocator_arg);
    }

    template <class Deleter, class T>
    Deleter* get_deleter(const shared_ptr<T>& r) {
        return r._cnt ? reinterpret_cast<Deleter*>(r._cnt.get_deleter()) : nullptr;
    }

    template <class T, class U>
    bool operator==(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) {
        return lhs.get() == rhs.get();
    }

    template <class T, class U>
    bool operator!=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) {
        return !(lhs == rhs);
    }

    template <class T, class U>
    bool operator< (const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) {
        return lhs.get() < rhs.get();
    }

    template <class T, class U>
    bool operator> (const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) {
        return rhs < lhs;
    }

    template <class T, class U>
    bool operator<=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) {
        return !(rhs < lhs);
    }

    template <class T, class U>
    bool operator>=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) {
        return !(lhs < rhs);
    }

    template <class T>
    bool operator==(const shared_ptr<T>& lhs, nullptr_t) {
        return !lhs;
    }
    template <class T>
    bool operator==(nullptr_t, const shared_ptr<T>& rhs) {
        return !rhs;
    }

    template <class T>
    bool operator!=(const shared_ptr<T>& lhs, nullptr_t) {
        return (bool) lhs;
    }

    template <class T>
    bool operator!=(nullptr_t, const shared_ptr<T>& rhs) {
        return (bool) rhs;
    }

    template <class T>
    bool operator< (const shared_ptr<T>& lhs, nullptr_t) {
        return lhs.get() < nullptr;
    }

    template <class T>
    bool operator< (nullptr_t, const shared_ptr<T>& rhs) {
        return nullptr < rhs.get();
    }

    template <class T>
    bool operator> (const shared_ptr<T>& lhs, nullptr_t) {
        return nullptr < lhs;
    }

    template <class T>
    bool operator> (nullptr_t, const shared_ptr<T>& rhs) {
        return rhs < nullptr;
    }

    template <class T>
    bool operator<=(const shared_ptr<T>& lhs, nullptr_t) {
        return !(nullptr < lhs);
    }

    template <class T>
    bool operator<=(nullptr_t, const shared_ptr<T>& rhs) {
        return !(rhs < nullptr);
    }

    template <class T>
    bool operator>=(const shared_ptr<T>& lhs, nullptr_t) {
        return !(lhs < nullptr);
    }

    template <class T>
    bool operator>=(nullptr_t, const shared_ptr<T>& rhs) {
        return !(nullptr < rhs);
    }

//    template <class T, class U, class V>
//    basic_ostream<U, V>& operator<<(basic_ostream<U, V>& os, const shared_ptr<T>& ptr);

    template <class T>
    void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) {
        lhs.swap(rhs);
    }

    template <class T>
    class weak_ptr {
    public:
        typedef T       element_type;
    protected:
        element_type* _ptr;
        ref_count_base* _cnt;
    public:
        weak_ptr() = default;
        
        weak_ptr(const weak_ptr& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_wref(); }

        template <class Y>
        weak_ptr(const weak_ptr<Y>& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_wref(); }

        template <class Y>
        weak_ptr(const shared_ptr<Y>& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_wref(); }

        weak_ptr(weak_ptr&& r): _ptr(r._ptr), _cnt(r._cnt) { r._ptr = r._cnt = nullptr; }

        template <class Y>
        weak_ptr(weak_ptr<Y>&& r): _ptr(r._ptr), _cnt(r._cnt) { r._ptr = r._cnt = nullptr; }

        ~weak_ptr() { if (_cnt) _cnt -> dec_wref(); }

        weak_ptr& operator=(const weak_ptr& r) { weak_ptr<T>(r).swap(*this); }
        
        template <class Y>
        weak_ptr& operator=(const weak_ptr<Y>& r) { weak_ptr<T>(r).swap(*this); }

        template <class Y>
        weak_ptr& operator=(const shared_ptr<Y>& r) { weak_ptr<T>(r).swap(*this); }

        weak_ptr& operator=(weak_ptr&& r) { weak_ptr<T>(HxSTL::move(r)).swap(*this); }

        template <class Y>
        weak_ptr& operator=(weak_ptr<Y>&& r) { weak_ptr<T>(HxSTL::move(r)).swap(*this); }

        void reset() { weak_ptr().swap(*this); }

        void swap(weak_ptr& r) {
            HxSTL::swap(_ptr, r._ptr);
            HxSTL::swap(_cnt, r._cnt);
        }

        long use_count() const { return _cnt ? _cnt -> use_count() : 0; }

        bool expired() const { return use_count() == 0; } 

        shared_ptr<T> lock() const { return expired() ? shared_ptr<T>() : shared_ptr<T>(*this); }

        template <class Y>
        bool owner_before(const weak_ptr<Y>& other) const { return _cnt < other._cnt; }

        template <class Y>
        bool owner_before(const shared_ptr<Y>& other) const { return _cnt < other._cnt; }
    public:
        template <class Y>
        friend class weak_ptr;
        template <class Y>
        friend class shared_ptr;
    };

    template <class T>
    void swap(weak_ptr<T>& lhs, weak_ptr<T>& rhs) {
        lhs.swap(rhs);
    }

}


#endif
