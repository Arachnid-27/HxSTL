#ifndef _POOL_ALLOCATOR_H_
#define _POOL_ALLOCATOR_H_


#include <cstddef>
#include "alloc.h"
#include "construct.h"


namespace HxSTL {

    template <class T>
    class pool_allocator {
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
            typedef pool_allocator<U> other;
        };
    public:
        pool_allocator() {}

        pool_allocator(const pool_allocator&) {}

        template <class U>
        pool_allocator(const pool_allocator<U>&) {}

        pointer address(reference x) const { return static_cast<pointer>(&x); }

        const_pointer address(const_reference x) const { return static_cast<const_pointer>(&x); }

        pointer allocate(size_type n) { return static_cast<pointer>(alloc::allocate(n * sizeof(T))); }

        void deallocate(pointer p, size_type n) { alloc::deallocate(p, n * sizeof(T)); }

        size_type max_size() const { return size_type(-1) / sizeof(T); }

        template <class U, class... Args>
        void construct(U* p, Args&&... args) { HxSTL::construct(p, args...); }

        template <class U>
        void destroy(U* p) { HxSTL::destroy(p); }
    };

}


#endif
