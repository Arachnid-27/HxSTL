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
 
    template <class T>
    inline typename allocator<T>::pointer allocator<T>::address(reference x) const {
        return static_cast<pointer>(&x);
    }

    template <class T>
    inline typename allocator<T>::const_pointer allocator<T>::address(const_reference x) const {
        return static_cast<const_pointer>(&x);
    }

    template <class T>
    inline typename allocator<T>::pointer allocator<T>::allocate(size_type n) {
        return static_cast<pointer>(alloc::allocate(n * sizeof(T)));
    }

    template <class T>
    inline void allocator<T>::deallocate(pointer p, size_type n) {
        return alloc::deallocate(p, n * sizeof(T));
    }

    template <class T>
    inline typename allocator<T>::size_type allocator<T>::max_size() const {
        return size_type(-1) / sizeof(T);
    }

    template <class T>
    inline void allocator<T>::construct(pointer p, const_reference val) {
        construct(p, val);
    }

    template <class T>
    inline void allocator<T>::destroy(pointer p) {
        destroy(p);
    }

}


#endif
