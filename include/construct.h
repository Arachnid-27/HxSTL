#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_


#include <new>
#include "type_traits.h"
#include "iterator.h"


namespace HxSTL {

    template <class T1, class T2>
    inline void construct(T1 *p, const T2 &val) {
        new (p) T1(val);     // placement new 
    }

    template <class T>
    inline void destroy(T *p) {
        p->~T();
    }

    template <class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last) {
        typedef typename iterator_traits<ForwardIterator>::value_type value_type;
        __destroy(first, last, typename is_pod<value_type>::value());
    }

    // has non-trivial destructor
    template <class ForwardIterator>
    inline void __destroy(ForwardIterator first, ForwardIterator last, false_type) {
        for (; first != last; ++first) {
            destroy(&*first);
        }
    }

    // has trivial destructor
    template <class ForwardIterator>
    inline void __destroy(ForwardIterator first, ForwardIterator last, true_type) {}

}


#endif
