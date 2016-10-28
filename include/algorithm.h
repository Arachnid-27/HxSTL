#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_


#include <cstring>
#include "type_traits.h"
#include "iterator.h"


namespace HxSTL {

    /*
     * copy
     */

    template <class InputIterator, class OutputIterator>
    struct __copy_dispath {
        OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
            return __copy(first, last, result, 
                    typename iterator_traits<InputIterator>::iterator_category());
        }
    };

    template <class T>
    struct __copy_dispath<T*, T*> {
        T* operator()(T* first, T* last, T* result) {
            return __copy_ptr(first, last, result, typename is_pod<T>::value());
        }
    };

    template <class T>
    struct __copy_dispath<const T*, T*> {
        T* operator()(const T* first, const T* last, T* result) {
            return __copy_ptr(first, last, result, typename is_pod<T>::value());
        }
    };

    template <class InputIterator, class OutputIterator>
    inline OutputIterator __copy(InputIterator first, InputIterator last, 
            OutputIterator result, input_iterator_tag) {
        while (first != last) {
            *result = *first;
            ++result;
            ++first;
        }
        return result;
    }

    template <class RandomAccessIterator, class OutputIterator>
    inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, 
            OutputIterator result, random_access_iterator_tag) {
        return __copy_rand(first, last, result);
    }

    template <class RandomAccessIterator, class OutputIterator>
    inline OutputIterator __copy_rand(RandomAccessIterator first, RandomAccessIterator last, 
            OutputIterator result) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type distance;
        for (distance n = last - first; n > 0; --n, ++result, ++first) {
            *result = *first;
        }
        return result;
    }

    template <class T>
    inline T* __copy_ptr(const T* first, const T* last, T* result, true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }
    
    template <class T>
    inline T* __copy_ptr(const T* first, const T* last, T* result, false_type) {
        return __copy_rand(first, last, result, static_cast<ptrdiff_t*>(0));
    }

    template <class InputIterator, class OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, 
            OutputIterator result) {
        return __copy_dispath<InputIterator, OutputIterator>()(first, last, result);
    }

    /*
     * copy_n
     */

    template <class InputIterator, class Size, class OutputIterator>
    struct __copy_n_dispath {
        OutputIterator operator()(InputIterator first, Size n, OutputIterator result) {
            return __copy_n(first, n, result, 
                    typename iterator_traits<InputIterator>::iterator_category());
        }
    };

    template <class T, class Size>
    struct __copy_n_dispath<T*, Size, T*> {
        T* operator()(T* first, Size n, T* result) {
            return __copy_n_ptr(first, n, result, typename is_pod<T>::value());
        }
    };

    template <class T, class Size>
    struct __copy_n_dispath<const T*, Size, T*> {
        T* operator()(const T* first, Size n, T* result) {
            return __copy_n_ptr(first, n, result, typename is_pod<T>::value());
        }
    };

    template <class T, class Size>
    inline T* __copy_n_ptr(const T* first, Size n, T* result, true_type) {
        memmove(result, first, sizeof(T) * n);
        return result + n;
    }
    
    template <class T, class Size>
    inline T* __copy_n_ptr(const T* first, Size n, T* result, false_type) {
        return __copy_n(first, n, result, static_cast<ptrdiff_t*>(0));
    }

    template <class InputIterator, class Size, class OutputIterator>
    inline OutputIterator __copy_n(InputIterator first, Size n, OutputIterator result) {
        while (n > 0) {
            *result = *first;
            --n;
            ++result;
            ++first;
        }
        return result;
    }

    template <class InputIterator, class Size, class OutputIterator>
    inline OutputIterator copy_n(InputIterator first, Size n, OutputIterator result) {
        return __copy_n_dispath<InputIterator, Size, OutputIterator>()(first, n, result);
    }

    /*
     * copy_backward
     */

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    struct __copy_backward_dispath {
        BidirectionalIterator2 operator()(BidirectionalIterator1 first, BidirectionalIterator1 last, 
                BidirectionalIterator2 result) {
            return __copy_backward(first, last, result, 
                    typename iterator_traits<BidirectionalIterator1>::iterator_category());
        }
    };

    template <class T>
    struct __copy_backward_dispath<T*, T*> {
        T* operator()(T* first, T* last, T* result) {
            return __copy_backward_ptr(first, last, result, typename is_pod<T>::value());
        }
    };

    template <class T>
    struct __copy_backward_dispath<const T*, T*> {
        T* operator()(const T* first, const T* last, T* result) {
            return __copy_backward_ptr(first, last, result, typename is_pod<T>::value());
        }
    };

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, 
            BidirectionalIterator2 result, bidirectional_iterator_tag) {
        while (first != last) {
            *(--result) = *(--last);
        }
        return result;
    }

    template <class RandomAccessIterator, class BidirectionalIterator>
    inline BidirectionalIterator __copy_backward(RandomAccessIterator first, RandomAccessIterator last, 
            BidirectionalIterator result, random_access_iterator_tag) {
        return __copy_backward_rand(first, last, result);
    }

    template <class RandomAccessIterator, class BidirectionalIterator>
    inline BidirectionalIterator __copy_backward_rand(RandomAccessIterator first, RandomAccessIterator last, 
            BidirectionalIterator result) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type distance;
        for (distance n = last - first; n > 0; --n) {
            *(--result) = *(--last);
        }
        return result;
    }

    template <class T>
    inline T* __copy_backward_ptr(const T* first, const T* last, T* result, true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }
    
    template <class T>
    inline T* __copy_backward_ptr(const T* first, const T* last, T* result, false_type) {
        return __copy_rand(first, last, result, static_cast<ptrdiff_t*>(0));
    }

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, 
            BidirectionalIterator1 last, BidirectionalIterator2 result) {
        return __copy_backward_dispath<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
    }

    /*
     * fill
     */

    template <class ForwardIterator, class T>
    inline void fill(ForwardIterator first, ForwardIterator last, const T& val) {
        while (first != last) {
            *first = val;
            ++first;
        }
    }

    template <class OutputIterator, class Size, class T>
    inline OutputIterator fill_n(OutputIterator first, Size n, const T& val) {
        while (n > 0) {
            *first = val;
            ++first;
            --n;
        }
        return first;
    }

}


#endif
