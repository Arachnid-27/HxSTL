#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_


#include "construct.h"
#include "iterator.h"
#include "algorithm.h"


namespace HxSTL {

    // uninitialized_copy

    template <bool Trivial>
    struct __uninitialized_copy {
        template <class InputIt, class ForwardIt>
        static ForwardIt __copy(InputIt first, InputIt last,
            ForwardIt result) {
            for (; first != last; ++first, ++result) {
                HxSTL::construct(result, *first);
            }
            return result;
        }
    };

    template <>
    struct __uninitialized_copy<true> {
        template <class InputIt, class ForwardIt>
        static ForwardIt __copy(InputIt first, InputIt last,
            ForwardIt result) {
            return HxSTL::copy(first, last, result);
        }
    };

    template <class InputIt, class ForwardIt>
    inline ForwardIt uninitialized_copy(InputIt first, InputIt last,
        ForwardIt result) {
        typedef typename iterator_traits<ForwardIt>::value_type value_type1;
        typedef typename iterator_traits<InputIt>::value_type value_type2;
        return __uninitialized_copy<HxSTL::is_pod<value_type1>::value &&
            HxSTL::is_pod<value_type2>::value>::__copy(first, last, result);
    }

    // uninitialized_move

    template <bool Trivial>
    struct __uninitialized_move {
        template <class InputIt, class ForwardIt>
        static ForwardIt __move(InputIt first, InputIt last,
            ForwardIt result) {
            for (; first != last; ++first, ++result) {
                HxSTL::construct(result, HxSTL::move(*first));
            }
            return result;
        }
    };

    template <>
    struct __uninitialized_move<true> {
        template <class InputIt, class ForwardIt>
        static ForwardIt __move(InputIt first, InputIt last,
            ForwardIt result) {
            return HxSTL::move(first, last, result);
        }
    };

    template <class InputIt, class ForwardIt>
    inline ForwardIt uninitialized_move(InputIt first, InputIt last,
        ForwardIt result) {
        typedef typename iterator_traits<ForwardIt>::value_type value_type1;
        typedef typename iterator_traits<InputIt>::value_type value_type2;
        return __uninitialized_move<HxSTL::is_pod<value_type1>::value &&
            HxSTL::is_pod<value_type2>::value>::__move(first, last, result);
    }

    // uninitialized_copy_n

    template <bool Trivial>
    struct __uninitialized_copy_n {
        template <class InputIt, class Size, class ForwardIt>
        static ForwardIt __copy_n(InputIt first, Size n,
            ForwardIt result) {
            for (; n != 0; --n, ++first, ++result) {
                HxSTL::construct(result, *first);
            }
            return result;
        }
    };

    template <>
    struct __uninitialized_copy_n<true> {
        template <class InputIt, class Size, class ForwardIt>
        static ForwardIt __copy_n(InputIt first, Size n,
            ForwardIt result) {
            return HxSTL::copy_n(first, n, result);
        }
    };

    template <class InputIt, class Size, class ForwardIt>
    inline ForwardIt uninitialized_copy_n(InputIt first, Size n,
        ForwardIt result) {
        typedef typename iterator_traits<ForwardIt>::value_type value_type1;
        typedef typename iterator_traits<InputIt>::value_type value_type2;
        return __uninitialized_copy_n<HxSTL::is_pod<value_type1>::value &&
            HxSTL::is_pod<value_type2>::value>::__copy_n(first, n, result);
    }

    // uninitialized_fill

    template <class ForwardIt, class T>
    inline void uninitialized_fill(ForwardIt first, ForwardIt last,
            const T& x) {
        typedef typename iterator_traits<ForwardIt>::value_type value_type;
        __uninitialized_fill(first, last, x, typename is_pod<value_type>::type());
    }

    template <class ForwardIt, class T>
    inline void __uninitialized_fill(ForwardIt first, ForwardIt last,
            const T& x, true_type) {
        fill(first, last, x);
    }

    template <class ForwardIt, class T>
    inline void __uninitialized_fill(ForwardIt first, ForwardIt last,
            const T& x, false_type) {
        while (first != last) {
            construct(first, x);
            ++first;
        }
    }

    // uninitialized_fill_n

    template <class ForwardIt, class Size, class T>
    inline ForwardIt uninitialized_fill_n(ForwardIt first, Size n, const T& x) {
        typedef typename iterator_traits<ForwardIt>::value_type value_type;
        return __uninitialized_fill_n(first, n, x, typename is_pod<value_type>::type());
    }

    template <class ForwardIt, class Size, class T>
    inline ForwardIt __uninitialized_fill_n(ForwardIt first, Size n, const T& x, true_type) {
        return fill_n(first, n, x);
    }

    template <class ForwardIt, class Size, class T>
    inline ForwardIt __uninitialized_fill_n(ForwardIt first, Size n, const T& x, false_type) {
        while (n > 0) {
            construct(first, x);
            --n;
            ++first;
        }
        return first;
    }

}


#endif
