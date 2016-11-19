#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_


#include <new>
#include "type_traits.h"
#include "algorithm.h"


namespace HxSTL {

    template <class U, class... Args>
    void construct(U* p, Args&&... args) {
        new(p) U(forward<Args>(args)...);
    }

    template <class T>
    inline void destroy(T *p) {
        p->~T();
    }

    template <class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last) {
        typedef typename iterator_traits<ForwardIterator>::value_type value_type;
        __destroy(first, last, typename HxSTL::is_trivially_destructible<value_type>::type());
    }

    template <class ForwardIterator>
    inline void __destroy(ForwardIterator first, ForwardIterator last, false_type) {
        for (; first != last; ++first) {
            destroy(&*first);
        }
    }

    template <class ForwardIterator>
    inline void __destroy(ForwardIterator first, ForwardIterator last, true_type) {}

}


#endif
