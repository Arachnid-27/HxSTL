#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_


#include <cstddef>
#include "construct.h"
#include "stdexcept.h"


#ifdef _USE_POOL_ALLOC
#include "pool_allocator.h"
#else
#include "new_allocator.h"
#endif


namespace HxSTL {

#ifdef _USE_POOL_ALLOC
template <class T>
using __base_allocator = HxSTL::pool_allocator<T>;
#else
template <class T>
using __base_allocator = HxSTL::new_allocator<T>;
#endif

    template <class T>
    class allocator:public __base_allocator<T> {
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
        allocator() {} 

        allocator(const allocator& alloc): __base_allocator<T>(alloc) {}

        template <class U>
        allocator(const allocator<U>& alloc): __base_allocator<T>(alloc) {}
    };

    struct allocator_arg_t {};

    constexpr allocator_arg_t allocator_arg = allocator_arg_t();

}


#endif
