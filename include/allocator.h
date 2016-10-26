#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_


#include <cstddef>
#include <climits>
#include "alloc.h"
#include "construct.h"


namespace HxSTL {

    template <class T>
    class allocator {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef T&          reference;
        typedef const T*    const_pointer;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        template <class U>
        struct rebind {
            typedef allocator<U> other;
        };
    public:
        pointer address(reference x) const;
        const_pointer address(const_reference x) const; 
        pointer allocate(size_type n);
        void deallocate(pointer p, size_type n);
        size_type max_size() const;
        void construct(pointer p, const_reference val);
        void destroy(pointer p);
    };
 
}


#endif
