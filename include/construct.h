#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_


#include <new>
#include "type_traits.h"
#include "algorithm.h"


namespace HxSTL {

    template <class U, class... Args>
    inline void construct(U* p, Args&&... args) {
        new(p) U(HxSTL::forward<Args>(args)...);
    }

    template <class T>
    inline void destroy(T *p) {
        p->~T();
    }

    template <class Alloc, class ForwardIt>
    inline void destroy(Alloc& alloc, ForwardIt first, ForwardIt last) {
        typedef typename HxSTL::iterator_traits<ForwardIt>::value_type value_type;
        __destroy(alloc, first, last, typename HxSTL::is_trivially_destructible<value_type>::type());
    }

    template <class Alloc, class ForwardIt>
    inline void __destroy(Alloc& alloc, ForwardIt first, ForwardIt last, HxSTL::false_type) {
        while (first != last) {
            alloc.destroy(&*(first++));
        }
    }

    template <class Alloc, class ForwardIt>
    inline void __destroy(Alloc&, ForwardIt, ForwardIt, HxSTL::true_type) {}

}


#endif
