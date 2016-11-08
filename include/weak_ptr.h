#ifndef _WEAK_PTR_
#define _WEAK_PTR_


#include "shared_ptr.h"


namespace HxSTL {

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

        ~weak_ptr() { _cnt -> dec_wref(); }

        weak_ptr& operator=(const weak_ptr& r) { weak_ptr<T>(r).swap(*this); }
        
        template <class Y>
        weak_ptr& operator=(const weak_ptr<Y>& r) { weak_ptr<T>(r).swap(*this); }

        template <class Y>
        weak_ptr& operator=(const shared_ptr<Y>& r) { weak_ptr<T>(r).swap(*this); }

        weak_ptr& operator=(weak_ptr&& r) { weak_ptr<T>(move(r)).swap(*this); }

        template <class Y>
        weak_ptr& operator=(weak_ptr<Y>&& r) { weak_ptr<T>(move(r)).swap(*this); }

        void reset() { weak_ptr().swap(*this); }

        void swap(weak_ptr& r) {
            HxSTL::swap(_ptr, r._ptr);
            HxSTL::swap(_cnt, r._cnt);
        }

        long use_count() const { return _cnt ? 0 : _cnt -> use_count(); }

        bool expired() const { return use_count() == 0; } 

        shared_ptr<T> lock() const { return expired() ? shared_ptr<T>() : shared_ptr<T>(*this); }

        template <class Y>
        bool owner_before(const weak_ptr<Y>& other) const { return _cnt < other._cnt; }

        template <class Y>
        bool owner_before(const shared_ptr<Y>& other) const { return _cnt < other._cnt; }
    };

    template <class T>
    void swap(weak_ptr<T>& lhs, weak_ptr<T>& rhs) {
        lhs.swap(rhs);
    }

}


#endif
