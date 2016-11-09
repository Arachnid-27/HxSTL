#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_


#include <cstddef>
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
        allocator() = default;

        allocator(const allocator&) {}

        template <class U>
        allocator(const allocator<U>&) {}

        pointer address(reference x) const { return static_cast<pointer>(&x); }

        const_pointer address(const_reference x) const { return static_cast<const_pointer>(&x); }

        pointer allocate(size_type n) { return static_cast<pointer>(alloc::allocate(n * sizeof(T))); }

        void deallocate(pointer p, size_type n) { return alloc::deallocate(p, n * sizeof(T)); }

        size_type max_size() const { return size_type(-1) / sizeof(T); }

        template <class U, class... Args>
        void construct(U* p, Args&&... args) { HxSTL::construct(p, args...); }

        template <class U>
        void destroy(U* p) { HxSTL::destroy(p); }
    };

    struct allocator_arg_t {};

    constexpr allocator_arg_t allocator_arg = allocator_arg_t();

}


#endif
