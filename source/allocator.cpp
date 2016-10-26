#include "allocator.h"


namespace HxSTL {

    template <class T>
    inline typename allocator<T>::pointer allocator<T>::address(reference x) const {
        return (pointer) &x;
    }

    template <class T>
    inline typename allocator<T>::const_pointer allocator<T>::address(const_reference x) const {
        return (const_pointer) &x;
    }

    template <class T>
    inline typename allocator<T>::pointer allocator<T>::allocate(size_type n) {
        return alloc::allocate(n);
    }

    template <class T>
    inline void allocator<T>::deallocate(pointer p, size_type n) {
        return alloc::deallocate(p, n);
    }

    template <class T>
    inline typename allocator<T>::size_type allocator<T>::max_size() const {
        return UINT_MAX / sizeof(T);
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
