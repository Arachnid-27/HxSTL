#ifndef _ITERATOR_H_
#define _ITERATOR_H_


#include <cstddef>


namespace HxSTL {

    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    
    template <class Category, class T, class Distance = ptrdiff_t, 
             class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
        typedef Category    iterator_category;
    };

    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::difference_type      difference_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
        typedef typename Iterator::iterator_category    iterator_category;
    };

    template <class T>
    struct iterator_traits<T*> {
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef T*                              pointer;
        typedef T&                              reference;
        typedef random_access_iterator_tag      iterator_category;
    };

    template <class T>
    struct iterator_traits<const T*> {
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef const T*                        pointer;
        typedef const T&                        reference;
        typedef random_access_iterator_tag      iterator_category;
    };

    template <class InputIterator, class Distance>
    inline void advance(InputIterator& it, Distance n) {
        __advance(it, n, typename iterator_traits<InputIterator>::iterator_category());
    }

    template <class InputIterator, class Distance>
    inline void __advance(InputIterator& it, Distance n, input_iterator_tag) {
        while (n--) ++it;
    }

    template <class BidirectionalIterator, class Distance>
    inline void __advance(BidirectionalIterator& it, Distance n, bidirectional_iterator_tag) {
        if (n >= 0) {
            while (n--) ++it;
        } else {
            while (n++) --it;
        }
    }

    template <class RandomAccessIterator, class Distance>
    inline void __advance(RandomAccessIterator& it, Distance n, random_access_iterator_tag) {
        it += n;
    }

    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
        __distance(first, last, typename iterator_traits<InputIterator>::iterator_category());
    }

    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last) {
            ++first;
            ++n;
        } 
        return n;
    }
    
    template <class RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
    __distance(RandomAccessIterator first, RandomAccessIterator last) {
        return last - first;
    }

}


#endif
