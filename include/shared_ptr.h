#ifndef _SHARED_PTR_
#define _SHARED_PTR_


#include <cstddef>
#include "type_traits.h"
#include "default_delete.h"
#include "utility.h"
#include "weak_ptr.h"
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

        virtual void* get_deleter() { return 0; }

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

        bool expired() const { return _use == 0; }
    };

    template <class T>
    class ref_count: public ref_count_base {
    private:
        virtual void destroy_object() { delete _ptr; }
        virtual void destory_this() { delete this; }
    protected:
        T* _ptr;
    public:
        ref_count(T* p): _ptr(p) {}
    };

    template <class T, class Deleter>
    class ref_count_del: public ref_count_base {
    private:
        virtual void destroy_object() { _del ? _del( _ptr) : delete _ptr; }
        virtual void destory_this() { delete this; }
    protected:
        T* _ptr;
        Deleter _del;
    public:
        ref_count_del(T* p, Deleter d): _ptr(p), _del(d) {}

        virtual void* get_deleter() { return &_del; }
    };

    template <class T, class Deleter, class Alloc>
    class ref_count_del_alloc: public ref_count_base {
    private:
        virtual void destroy_object() { _del ? _del(_ptr) : delete _ptr; }
        virtual void destory_this() {
            typename Alloc::template rebind<ref_count_del_alloc>::other tmp_alloc(_alloc);
            tmp_alloc.destroy(this);
            tmp_alloc.deallocate(this, 1);
        }
    protected:
        T* _ptr;
        Deleter _del;
        Alloc _alloc;
    public:
        ref_count_del_alloc(T* p, Deleter d, const Alloc& alloc): _ptr(p), _del(d), _alloc(alloc) {}

        virtual void* get_deleter() { return &_del; }
    };

    template <class T>
    class shared_ptr {
    public:
        typedef T           element_type;
    protected:
        element_type* _ptr;
        ref_count_base* _cnt;
    public:
        shared_ptr() = default;

        shared_ptr(nullptr_t) : shared_ptr() {}

        template <class Y>
        shared_ptr(Y* p): _ptr(p), _cnt(new ref_count<T>(_ptr)) {}

        template <class Y, class Deleter>
        shared_ptr(Y* p, Deleter d): _ptr(p), _cnt(new ref_count_del<T, Deleter>(_ptr, d)) {}

        template <class Deleter>
        shared_ptr(nullptr_t p, Deleter d): _ptr(p), _cnt(new ref_count_del<T, Deleter>(_ptr, d)) {}

        template <class Y, class Deleter, class Alloc>
        shared_ptr(Y* p, Deleter d, Alloc alloc): _ptr(p) {
            typename Alloc::template rebind<ref_count_del_alloc<T, Deleter, Alloc>>::other tmp_alloc(alloc);
            _cnt = tmp_alloc.allocate(1);
            tmp_alloc.construct(_cnt, _ptr, d, alloc);
        }

        template <class Deleter, class Alloc>
        shared_ptr(nullptr_t p, Deleter d, Alloc alloc): _ptr(p) {
            typename Alloc::template rebind<ref_count_del_alloc<T, Deleter, Alloc>>::other tmp_alloc(alloc);
            _cnt = tmp_alloc.allocate(1);
            tmp_alloc.construct(_cnt, _ptr, d, alloc);
        }

        template <class Y>
        shared_ptr(const shared_ptr<Y>& r, element_type* p): _ptr(p), _cnt(r._cnt) { _cnt -> inc_ref(); }

        shared_ptr(const shared_ptr& r): _ptr(r._Ptr), _cnt(r._cnt) { _cnt -> inc_ref(); }

        template <class Y>
        shared_ptr(const shared_ptr<Y>& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_ref(); }

        shared_ptr(shared_ptr&& r): _ptr(r._ptr), _cnt(r._cnt) { r._ptr = r.cnt = nullptr; }

        template <class Y>
        shared_ptr(shared_ptr&& r): _ptr(r._ptr), _cnt(r._cnt) { r._ptr = r._cnt = nullptr; }

        template <class Y>
        explicit shared_ptr(const weak_ptr<Y>& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_ref(); }

        template <class Y, class Deleter>
        shared_ptr(unique_ptr<Y, Deleter>&& r)
        : _ptr(r.release()), _cnt(new ref_count_del<T, Deleter>(_ptr, r.get_deleter())) {}

        ~shared_ptr() { _cnt -> dec_ref(); }

        shared_ptr& operator=(const shared_ptr& r) {
            shared_ptr<T>(r).swap(*this);
            return *this;
        }

        template <class Y>
        shared_ptr& operator=(const shared_ptr<Y>& r) {
            shared_ptr<T>(r).swap(*this);
            return *this;
        }

        shared_ptr& operator=(shared_ptr&& r) {
            shared_ptr<T>(move(r)).swap(*this);
            return *this;
        }

        template <class Y>
        shared_ptr& operator=(shared_ptr<Y>&& r) {
            shared_ptr<T>(move(r)).swap(*this);
            return *this;
        }

        template <class Y, class Deleter>
        shared_ptr& operator=(unique_ptr<Y, Deleter>&& r) {
            shared_ptr<T>(move(r)).swap(*this);
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

        long use_count() const { return _cnt ? 0 : _cnt -> use_count(); }

        bool unique() const { return use_count() == 1; }

        explicit operator bool() const { return get() != nullptr; }

        template <class Y>
        bool owner_before(const shared_ptr<Y>& other) const { return _cnt < other._cnt; }

        template <class Y>
        bool owner_before(const weak_ptr<Y>& other) const { return _cnt < other._cnt; }
    public:
        template <class Deleter, class Y>
        friend Deleter* get_deleter(const shared_ptr<Y>& p);
    };

    template <class T, class... Args>
    shared_ptr<T> make_shared(Args&&... args) {
        return new shared_ptr<T>(new T(args...));
    }

    template <class T, class Alloc, class... Args>
    shared_ptr<T> allocate_shared(const Alloc& alloc, Args&&... args) {
        return new shared_ptr<T>(new T(args...), 0, alloc);
    }

    template <class Deleter, class T>
    Deleter* get_deleter(const shared_ptr<T>& r) {
        return r._cnt ? nullptr : static_cast<Deleter*>(r._cnt.get_deleter());
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
    bool operator==(const shared_ptr<T>& lhs, std::nullptr_t) {
        return !lhs;
    }
    template <class T>
    bool operator==(std::nullptr_t, const shared_ptr<T>& rhs) {
        return !rhs;
    }

    template <class T>
    bool operator!=(const shared_ptr<T>& lhs, std::nullptr_t) {
        return (bool) lhs;
    }

    template <class T>
    bool operator!=(std::nullptr_t, const shared_ptr<T>& rhs) {
        return (bool) rhs;
    }

    template <class T>
    bool operator< (const shared_ptr<T>& lhs, std::nullptr_t) {
        return lhs.get() < nullptr;
    }

    template <class T>
    bool operator< (std::nullptr_t, const shared_ptr<T>& rhs) {
        return nullptr < rhs.get();
    }

    template <class T>
    bool operator> (const shared_ptr<T>& lhs, std::nullptr_t) {
        return nullptr < lhs;
    }

    template <class T>
    bool operator> (std::nullptr_t, const shared_ptr<T>& rhs) {
        return rhs < nullptr;
    }

    template <class T>
    bool operator<=(const shared_ptr<T>& lhs, std::nullptr_t) {
        return !(nullptr < lhs);
    }

    template <class T>
    bool operator<=(std::nullptr_t, const shared_ptr<T>& rhs) {
        return !(rhs < nullptr);
    }

    template <class T>
    bool operator>=(const shared_ptr<T>& lhs, std::nullptr_t) {
        return !(lhs < nullptr);
    }

    template <class T>
    bool operator>=(std::nullptr_t, const shared_ptr<T>& rhs) {
        return !(nullptr < rhs);
    }

//    template <class T, class U, class V>
//    basic_ostream<U, V>& operator<<(basic_ostream<U, V>& os, const shared_ptr<T>& ptr);

    template <class T>
    void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) {
        lhs.swap(rhs);
    }

}


#endif
