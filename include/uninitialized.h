#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_


#include "construct.h"
#include "iterator.h"
#include "algorithm.h"


namespace HxSTL {

    template <class InputIterator, class ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
            ForwardIterator result) {
        typedef typename iterator_traits<ForwardIterator>::value_type value_type;
        return __uninitialized_copy(first, last, result, typename is_pod<value_type>::value());
    }

    template <class InputIterator, class ForwardIterator>
    inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last,
            ForwardIterator result, true_type) {
        return copy(first, last, result);
    }

    template <class InputIterator, class ForwardIterator>
    inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last,
            ForwardIterator result, false_type) {
        for (; first != last; ++first, ++result) {
            construct(&*result, *first);
        }
        return result;
    }

    template <class ForwardIterator, class T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, 
            const T& x) {
        typedef typename iterator_traits<ForwardIterator>::value_type value_type;
        __uninitialized_fill(first, last, x, typename is_pod<value_type>::value());
    }

    template <class ForwardIterator, class T>
    inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, 
            const T& x, true_type) {
        fill(first, last, x);
    }

    template <class ForwardIterator, class T>
    inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, 
            const T& x, false_type) {
        for (; first != last; ++first) {
            constrcut(&*first, x);
        }
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
        typedef typename iterator_traits<ForwardIterator>::value_type value_type;
        return __uninitialized_fill_n(first, n, x, typename is_pod<value_type>::value());
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, true_type) {
        return fill_n(first, n, x);
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, false_type) {
        for (; n--; ++first) {
            constrcut(&*first, x);
        }
        return first;
    }

}


#endif
