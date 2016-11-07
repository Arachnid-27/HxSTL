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
        weak_ptr(): _ptr(nullptr), _cnt(nullptr) {}
        
        weak_ptr(const weak_ptr& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_wref(); }

        template <class Y>
        weak_ptr(const weak_ptr<Y>& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_wref(); }

        template <class Y>
        weak_ptr(const shared_ptr<Y>& r): _ptr(r._ptr), _cnt(r._cnt) { _cnt -> inc_wref(); }

        ~weak_ptr() { _cnt -> dec_wref(); }
    };

}


#endif
