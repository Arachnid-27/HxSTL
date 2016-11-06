#ifndef _UNIQUE_PTR_
#define _UNIQUE_PTR_


#include "algorithm.h"
#include "default_delete.h"


namespace HxSTL {

    template <class T, class Deleter = default_delete<T>>
    class unique_ptr {
    public:
        typedef T*              pointer;
        typedef T               element_type;
        typedef Deleter         deleter_type;
    protected:
        pointer _ptr;
        deleter_type _deleter;
    public:
        unique_ptr(): _ptr(nullptr), _deleter(Deleter()) {}

        explicit unique_ptr(pointer ptr): _ptr(ptr), _deleter(Deleter()) {}

        unique_ptr(pointer ptr, const deleter_type& deleter)
            :_ptr(ptr), _deleter(deleter) {}

        unique_ptr(const unique_ptr&) = delete;

        ~unique_ptr() { _deleter(_ptr); }

        unique_ptr& operator=(const unique_ptr&) = delete;

        explicit operator bool() const { return _ptr != nullptr; }
        element_type& operator*() const { return *_ptr; }
        pointer operator->() const { return _ptr; }

        pointer get() const { return _ptr; }
        deleter_type& get_deleter() { return _deleter; }
        const deleter_type& get_deleter() const { return _deleter; }
        pointer release();
        void reset(pointer ptr = pointer());
        void swap(unique_ptr& x) { HxSTL::swap(_ptr, x._ptr);; }
    };
    
    template <class T, class Deleter>
    typename unique_ptr<T, Deleter>::pointer unique_ptr<T, Deleter>::release() {
        pointer tmp = _ptr;
        _ptr = nullptr;
        return tmp;
    }

    template <class T, class Deleter>
    void unique_ptr<T, Deleter>::reset(pointer ptr) {
        pointer tmp = _ptr;
        _ptr = ptr;
        _deleter(tmp);
    }

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
