#ifndef _NEW_ALLOCATOR_H_
#define _NEW_ALLOCATOR_H_


#include <stddef.h>
#include "construct.h"
#include "stdexcept.h"


namespace HxSTL {

    template <class T>
    class new_allocator {
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
            typedef new_allocator<U> other;
        };
    public:
        new_allocator() {} 

        new_allocator(const new_allocator&) {}

        template <class U>
        new_allocator(const new_allocator<U>&) {}

        pointer address(reference x) const { return static_cast<pointer>(&x); }

        const_pointer address(const_reference x) const { return static_cast<const_pointer>(&x); }

        pointer allocate(size_type n) {
            if (n > max_size()) throw HxSTL::bad_exception();
            return static_cast<pointer>(::operator new(n * sizeof(T)));
        }

        void deallocate(pointer p, size_type) { ::operator delete(p); }

        size_type max_size() const { return size_type(-1) / sizeof(T); }

        template <class U, class... Args>
        void construct(U* p, Args&&... args) { HxSTL::construct(p, HxSTL::forward<Args>(args)...); }

        template <class U>
        void destroy(U* p) { HxSTL::destroy(p); }
    };

}


#endif
