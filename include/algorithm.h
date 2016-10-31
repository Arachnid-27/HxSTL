#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_


#include <cstring>
#include "utility.h"
#include "type_traits.h"
#include "iterator.h"


namespace HxSTL {

    template <class InputIterator, class UnaryPredicate>
    inline bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last) {
            if (!pred(*first)) {
                return false;
            }
            ++first;
        }
        return true;
    }

    template <class InputIterator, class UnaryPredicate>
    inline bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last) {
            if (pred(*first)) {
                return true;
            }
            ++first;
        }
        return false;
    }

    template <class InputIterator, class UnaryPredicate>
    inline bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last) {
            if (pred(*first)) {
                return false;
            }
            ++first;
        }
        return true;
    }

    template <class InputIterator, class Function>
    inline Function for_each(InputIterator first, InputIterator last, Function fn) {
        while (first != last) {
            fn(*first);
            ++first;
        }
        return fn;
    }

    template <class InputIterator, class T>
    inline InputIterator find(InputIterator first, InputIterator last, const T& val) {
        while (first != last && *first != val) {
            ++first;
        }
        return first;
    }

    template <class InputIterator, class UnaryPredicate>
    inline InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last && !pred(*first)) {
            ++first;
        }
        return first;
    }

    template <class InputIterator, class UnaryPredicate>
    inline InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last && pred(*first)) {
            ++first;
        }
        return first;
    }

    template <class ForwardIterator1, class ForwardIterator2>
    inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, ForwardIterator2 last2); 

    template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
    inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred); 

    template <class InputIterator, class ForwardIterator>
    inline InputIterator find_first_of(InputIterator first1, InputIterator last1, 
            ForwardIterator first2, ForwardIterator last2) {
        while (first1 != last1) {
            for (ForwardIterator it = first2; it != last2; ++it) {
                if (*it == *first1) {
                    return first1;
                }
            }
            ++first1;
        }
        return last1;
    }

    template <class InputIterator, class ForwardIterator, class BinaryPredicate>
    inline InputIterator find_first_of(InputIterator first1, InputIterator last1, 
            ForwardIterator first2, ForwardIterator last2, BinaryPredicate pred) {
        while (first1 != last1) {
            for (ForwardIterator it = first2; it != last2; ++it) {
                if (pred(*it, *first1)) {
                    return first1;
                }
            }
            ++first1;
        }
        return last1;
    }

    template <class ForwardIterator>
    inline ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
        if (first != last) {
            ForwardIterator next = first;
            ++next;
            while (next != last && *first != *next) {
                ++first;
                ++next;
            }
        }
        return first;
    }

    template <class ForwardIterator, class BinaryPredicate>
    inline ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate pred) {
        if (first != last) {
            ForwardIterator next = first;
            ++next;
            while (next != last && !pred(*first, *next)) {
                ++first;
                ++next;
            }
        }
        return first;
    }

    template <class InputIterator, class T>
    inline typename iterator_traits<InputIterator>::difference_type
    count(InputIterator first, InputIterator last, const T& val) {
        typename iterator_traits<InputIterator>::difference_type result = 0;
        while (first != last) {
            if (*first == val) {
                ++result;
            }
            ++first;
        }
        return result;
    }

    template <class InputIterator, class UnaryPredicate>
    inline typename iterator_traits<InputIterator>::difference_type
    count_if(InputIterator first, InputIterator last, UnaryPredicate pred) {
        typename iterator_traits<InputIterator>::difference_type result = 0;
        while (first != last) {
            if (pred(*first)) {
                ++result;
            }
            ++first;
        }
        return result;
    }

    template <class InputIterator1, class InputIterator2>
    inline pair<InputIterator1, InputIterator2>
    mismatch(InputIterator1 first1, InputIterator1 last1, 
            InputIterator2 first2) {
        while (first1 != last1 && *first1 == *first2) {
            ++first1;
            ++first2;
        }
        return make_pair(first1, first2);
    }

    template <class InputIterator1, class InputIterator2, class BinaryPredicate>
    inline pair<InputIterator1, InputIterator2>
    mismatch(InputIterator1 first1, InputIterator1 last1, 
            InputIterator2 first2, BinaryPredicate pred) {
        while (first1 != last1 && pred(*first1, *first2)) {
            ++first1;
            ++first2;
        }
        return make_pair(first1, first2);
    }

    template <class InputIterator1, class InputIterator2>
    inline bool equal(InputIterator1 first1, InputIterator1 last1, 
            InputIterator2 first2) {
        while (first1 != last1) {
            if (*first1 != *first2) {
                return false;
            }
            ++first1;
            ++first2;
        }
        return true;
    }

    template <class InputIterator1, class InputIterator2, class BinaryPredicate>
    inline bool equal(InputIterator1 first1, InputIterator1 last1, 
            InputIterator2 first2, BinaryPredicate pred) {
        while (first1 != last1) {
            if (!pred(*first1, *first2)) {
                return false;
            }
            ++first1;
            ++first2;
        }
        return true;
    }

    template <class ForwardIterator1, class ForwardIterator2>
    inline bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2);

    template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
    inline bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, BinaryPredicate pred);

    template <class ForwardIterator1, class ForwardIterator2>
    inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, ForwardIterator2 last2) {
        if (first2 == last2) {
            return first1;
        }

        ForwardIterator1 it1;
        ForwardIterator2 it2;
        while (first1 != last1) {
            it1 = first1;
            it2 = first2;
            while (*it1 == *it2) {
                ++it1;
                ++it2;
                if (it2 == last2) {
                    return first1;
                }
                if (it1 == last1) {
                    return last1;
                }
            }
            ++first1;
        }
        return last1;
    }

    template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
    inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred) {
        if (first2 == last2) {
            return first1;
        }

        ForwardIterator1 it1;
        ForwardIterator2 it2;
        while (first1 != last1) {
            it1 = first1;
            it2 = first2;
            while (pred(*it1, *it2)) {
                ++it1;
                ++it2;
                if (it2 == last2) {
                    return first1;
                }
                if (it1 == last1) {
                    return last1;
                }
            }
            ++first1;
        }
        return last1;
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator search_n(ForwardIterator first, ForwardIterator last, 
            Size count, const T& val) {
        ForwardIterator limit = first;
        advance(limit, distance(first, last) - count);
        
        ForwardIterator it;
        Size n;
        while (first != limit) {
            it = first;
            n = 0;
            while (*it == val) {
                ++it;
                if (++n == count) {
                    return first;
                }
            }
            ++first;
        }
        return first;
    }

    template <class ForwardIterator, class Size, class T, class BinaryPredicate>
    inline ForwardIterator search_n(ForwardIterator first, ForwardIterator last, 
            Size count, const T& val, BinaryPredicate pred) {
        ForwardIterator limit = first;
        advance(limit, distance(first, last) - count);
        
        ForwardIterator it;
        Size n;
        while (first != limit) {
            it = first;
            n = 0;
            while (pred(*it, val)) {
                ++it;
                if (++n == count) {
                    return first;
                }
            }
            ++first;
        }
        return first;
    }

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
