#ifndef _UNIQUE_PTR_
#define _UNIQUE_PTR_


#include "algorithm.h"


namespace HxSTL {

    template <class T>
    struct default_delete {
        default_delete() = default;

        void operator()(T* ptr) const { delete ptr; }
    };

    template <class T>
    struct default_delete<T[]> {
        default_delete() = default;

        void operator()(T* ptr) const { delete[] ptr; }
    };

    template <class T, class Deleter = default_delete<T>>
    class unique_ptr {
    public:
        typedef T*              pointer;
        typedef T               element_type;
        typedef Deleter         deleter_type;
    protected:
        pointer _ptr;
        deleter_type _del;
    public:
        unique_ptr(): _ptr(nullptr) {}

        unique_ptr(nullptr_t): _ptr(nullptr) {}

        explicit unique_ptr(pointer p): _ptr(p), _del(Deleter()) {}

        unique_ptr(pointer p, typename conditional<is_reference<Deleter>::value, 
                Deleter, const Deleter&>::type d):_ptr(p), _del(d) {}

        unique_ptr(pointer p, typename remove_reference<Deleter>::type&& d): _ptr(p), _del(move(d)) {}

        unique_ptr(unique_ptr&& u): _ptr(u._ptr) { u._ptr = nullptr; }

        unique_ptr(const unique_ptr&) = delete;

        ~unique_ptr() { if(_ptr) _del(_ptr); }

        unique_ptr& operator=(unique_ptr&& u) {
            _ptr = u._ptr;
            u._ptr = nullptr;
        }

        unique_ptr& operator=(nullptr_t) { _ptr = nullptr; }

        unique_ptr& operator=(const unique_ptr&) = delete;

        pointer release() {
            pointer old_ptr = _ptr;
            _ptr = nullptr;
            return old_ptr;
        }

        void reset(pointer p = pointer()) {
            pointer old_ptr = _ptr;
            _ptr = p;
            if (old_ptr != nullptr) _del(old_ptr);
        }

        void swap(unique_ptr& x) { HxSTL::swap(_ptr, x._ptr);; }

        pointer get() const { return _ptr; }

        deleter_type& get_deleter() { return _del; }

        const deleter_type& get_deleter() const { return _del; }

        explicit operator bool() const { return _ptr != nullptr; }

        element_type& operator*() const { return *_ptr; }

        pointer operator->() const { return _ptr; }
    };

    template <class T, class Deleter>
    class unique_ptr<T[], Deleter> {
    };

    template<class T1, class D1, class T2, class D2>
    bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
        return x.get() == y.get();
    }

    template<class T1, class D1, class T2, class D2>
    bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
        return x.get() != y.get();
    }

    template<class T1, class D1, class T2, class D2>
    bool operator< (const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
        return x.get() < y.get();
    }

    template<class T1, class D1, class T2, class D2>
    bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
        return !(y < x);
    }

    template<class T1, class D1, class T2, class D2>
    bool operator> (const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
        return y < x;
    }

    template<class T1, class D1, class T2, class D2>
    bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
        return !(x < y);
    }

    template <class T, class D>
    bool operator==(const unique_ptr<T, D>& x, nullptr_t) {
        return !x;
    }

    template <class T, class D>
    bool operator==(nullptr_t, const unique_ptr<T, D>& x) {
        return !x;
    }

    template <class T, class D>
    bool operator!=(const unique_ptr<T, D>& x, nullptr_t) {
        return (bool) x;
    }

    template <class T, class D>
    bool operator!=(nullptr_t, const unique_ptr<T, D>& x) {
        return (bool) x; 
    }

    template <class T, class D>
    bool operator< (const unique_ptr<T, D>& x, nullptr_t) {
        return x.get() < nullptr;
    }
    
    template <class T, class D>
    bool operator< (nullptr_t, const unique_ptr<T, D>& y) {
        return nullptr < y.get();
    }

    template <class T, class D>
    bool operator<=(const unique_ptr<T, D>& x, nullptr_t) {
        return nullptr < x;
    }

    template <class T, class D>
    bool operator<=(nullptr_t, const unique_ptr<T, D>& y) {
        return y < nullptr;
    }

    template <class T, class D>
    bool operator> (const unique_ptr<T, D>& x, nullptr_t) {
        return !(nullptr < x);
    }

    template <class T, class D>
    bool operator> (nullptr_t, const unique_ptr<T, D>& y) {
        return !(y < nullptr);
    }

    template <class T, class D>
    bool operator>=(const unique_ptr<T, D>& x, nullptr_t) {
        return !(x < nullptr);
    }

    template <class T, class D>
    bool operator>=(nullptr_t, const unique_ptr<T, D>& y) {
        return !(nullptr < y);
    }

    template <class T, class Deleter>
    void swap(unique_ptr<T, Deleter>& x, unique_ptr<T, Deleter>& y) {
        x.swap(y);
    }

}


#endif
