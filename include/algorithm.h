#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_


#include <cstring>
#include <cstdlib>
#include "utility.h"
#include "type_traits.h"
#include "iterator.h"


namespace HxSTL {

    /*
     * Non-modifying sequence operations
     */

    // all_of

    template <class InputIterator, class UnaryPredicate>
    bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last) {
            if (!pred(*first)) {
                return false;
            }
            ++first;
        }
        return true;
    }

    // any_of

    template <class InputIterator, class UnaryPredicate>
    bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last) {
            if (pred(*first)) {
                return true;
            }
            ++first;
        }
        return false;
    }

    // none_of

    template <class InputIterator, class UnaryPredicate>
    bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last) {
            if (pred(*first)) {
                return false;
            }
            ++first;
        }
        return true;
    }

    // for_each

    template <class InputIterator, class Function>
    Function for_each(InputIterator first, InputIterator last, Function fn) {
        while (first != last) {
            fn(*first);
            ++first;
        }
        return fn;
    }

    // find

    template <class InputIterator, class T>
    InputIterator find(InputIterator first, InputIterator last, const T& val) {
        while (first != last && *first != val) {
            ++first;
        }
        return first;
    }

    // find_if

    template <class InputIterator, class UnaryPredicate>
    InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last && !pred(*first)) {
            ++first;
        }
        return first;
    }

    // find_if_not

    template <class InputIterator, class UnaryPredicate>
    InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last && pred(*first)) {
            ++first;
        }
        return first;
    }

    // find_end

    template <class ForwardIterator1, class ForwardIterator2>
    ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, ForwardIterator2 last2) {
        return __find_end(first1, last1, first2, last2, 
                typename iterator_traits<ForwardIterator1>::iterator_category(), 
                typename iterator_traits<ForwardIterator2>::iterator_category());
    }

    template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
    ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred) {
        return __find_end(first1, last1, first2, last2, pred,
                typename iterator_traits<ForwardIterator1>::iterator_category(), 
                typename iterator_traits<ForwardIterator2>::iterator_category());
    }

    template <class ForwardIterator1, class ForwardIterator2>
    ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, ForwardIterator2 last2, 
            forward_iterator_tag, forward_iterator_tag) {
        ForwardIterator1 first = search(first1, last1, first2, last2);
        if (first != last1) {
            while (true) {
                ForwardIterator1 tmp = search(first, last1, first2, last2);
                if (tmp != last1) {
                    first = tmp;
                } else {
                    return first;
                }
            }
        }
        return first;
    }

    template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
    ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred, 
            forward_iterator_tag, forward_iterator_tag) {
        ForwardIterator1 first = search(first1, last1, first2, last2);
        if (first != last1) {
            while (true) {
                ForwardIterator1 tmp = search(first, last1, first2, last2, pred);
                if (tmp != last1) {
                    first = tmp;
                } else {
                    return first;
                }
            }
        }
        return first;
    }

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, 
            BidirectionalIterator2 first2, BidirectionalIterator2 last2, 
            bidirectional_iterator_tag, bidirectional_iterator_tag) {
        BidirectionalIterator1 tmp = last1;
        while (first1 != last1) {
            BidirectionalIterator1 it1 = last1;
            BidirectionalIterator2 it2 = last2;
            while (it2 != first2 && *it2 == *it1) {
                --it1;
                --it2;
                if (it2 == first2) {
                    return first2;
                }
                if (it1 == first1) {
                    return tmp;
                }
            }
            --last1;
        }
        return tmp;
    } 

    template <class BidirectionalIterator1, class BidirectionalIterator2, class BinaryPredicate>
    BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, 
            BidirectionalIterator2 first2, BidirectionalIterator2 last2, BinaryPredicate pred,
            bidirectional_iterator_tag, bidirectional_iterator_tag) {
        BidirectionalIterator1 tmp = last1;
        while (first1 != last1) {
            BidirectionalIterator1 it1 = last1;
            BidirectionalIterator2 it2 = last2;
            while (it2 != first2 && pred(*it1, *it2)) {
                --it1;
                --it2;
                if (it2 == first2) {
                    return first2;
                }
                if (it1 == first1) {
                    return tmp;
                }
            }
            --last1;
        }
        return tmp;
    } 

    // find_first_of

    template <class InputIterator, class ForwardIterator>
    InputIterator find_first_of(InputIterator first1, InputIterator last1, 
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
    InputIterator find_first_of(InputIterator first1, InputIterator last1, 
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

    // adjacent_find

    template <class ForwardIterator>
    ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
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
    ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate pred) {
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

    // count

    template <class InputIterator, class T>
    typename iterator_traits<InputIterator>::difference_type
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

    // count_if

    template <class InputIterator, class UnaryPredicate>
    typename iterator_traits<InputIterator>::difference_type
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

    // mismatch

    template <class InputIterator1, class InputIterator2>
    pair<InputIterator1, InputIterator2>
    mismatch(InputIterator1 first1, InputIterator1 last1, 
            InputIterator2 first2) {
        while (first1 != last1 && *first1 == *first2) {
            ++first1;
            ++first2;
        }
        return make_pair(first1, first2);
    }

    template <class InputIterator1, class InputIterator2, class BinaryPredicate>
    pair<InputIterator1, InputIterator2>
    mismatch(InputIterator1 first1, InputIterator1 last1, 
            InputIterator2 first2, BinaryPredicate pred) {
        while (first1 != last1 && pred(*first1, *first2)) {
            ++first1;
            ++first2;
        }
        return make_pair(first1, first2);
    }

    // equal

    template <class InputIterator1, class InputIterator2>
    bool equal(InputIterator1 first1, InputIterator1 last1, 
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
    bool equal(InputIterator1 first1, InputIterator1 last1, 
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

    // is_permutation

    template <class ForwardIterator1, class ForwardIterator2>
    bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2);

    template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
    bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1, 
            ForwardIterator2 first2, BinaryPredicate pred);

    // search
 
    template <class ForwardIterator1, class ForwardIterator2>
    ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, 
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
    ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, 
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

    // search_n

    template <class ForwardIterator, class Size, class T>
    ForwardIterator search_n(ForwardIterator first, ForwardIterator last, 
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
    ForwardIterator search_n(ForwardIterator first, ForwardIterator last, 
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
     * Modifying sequence operations
     */

    // copy

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
    OutputIterator copy(InputIterator first, InputIterator last, 
            OutputIterator result) {
        return __copy_dispath<InputIterator, OutputIterator>()(first, last, result);
    }

    // copy_n

    template <class InputIterator, class Size, class OutputIterator>
    struct __copy_n_dispath {
        OutputIterator operator()(InputIterator first, Size n, OutputIterator result) {
            return __copy_n(first, n, result);
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
    OutputIterator copy_n(InputIterator first, Size n, OutputIterator result) {
        return __copy_n_dispath<InputIterator, Size, OutputIterator>()(first, n, result);
    }

    // copy_if

    template <class InputIterator, class OutputIterator, class UnaryPredicate>
    OutputIterator copy_if(InputIterator first, InputIterator last, 
            OutputIterator result, UnaryPredicate pred) {
        while (first != last) {
            if (pred(*first)) {
                *result = *first;
                ++result;
            }
            ++first;
        }
        return result;
    }

    // copy_backward

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
    BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, 
            BidirectionalIterator1 last, BidirectionalIterator2 result) {
        return __copy_backward_dispath<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
    }

    // swap_range

    template <class ForwardIterator1, class ForwardIterator2>
    ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
        while (first1 != last1) {
            swap(*first1, *first2);
            ++first1;
            ++first2;
        }
        return first2;
    }

    // iter_swap

    template <class ForwardIterator1, class ForwardIterator2>
    void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
        swap(*a, *b);
    }

    // transform

    template <class InputIterator, class OutputIterator, class UnaryPredicate>
    OutputIterator transform(InputIterator first1, InputIterator last1, 
            OutputIterator result, UnaryPredicate op) {
        while (first1 != last1) {
            *result = op(first1);
            ++first1;
            ++result;
        }
        return result;
    }

    template <class InputIterator1, class InputIterator2, class OutputIterator, class BinaryPredicate>
    OutputIterator transform(InputIterator1 first1, InputIterator1 last1, 
            InputIterator2 first2, OutputIterator result, BinaryPredicate op) {
        while (first1 != last1) {
            *result = op(first1, first2);
            ++first1;
            ++first2;
            ++result;
        }
        return result;
    }
    
    // replace

    template <class ForwardIterator, class T>
    void replace(ForwardIterator first, ForwardIterator last, 
            const T& old_val, const T& new_val) {
        while (first != last) {
            if (*first == old_val) {
                *first = new_val;
            }
            ++first;
        }
    }

    // replace_if

    template <class InputIterator, class OutputIterator, class UnaryPredicate>
    void replace_if(InputIterator first, InputIterator last, 
            OutputIterator result, UnaryPredicate pred) {
        while (first != last) {
            if (pred(*first)) {
                *result = *first;
            }
            ++first;
            ++result;
        }
    }

    // replace_copy
    
    template <class InputIterator, class OutputIterator, class T>
    void replace_copy(InputIterator first, InputIterator last, 
            OutputIterator result, const T& old_val, const T& new_val) {
        while (first != last) {
            *result = *first == old_val ? new_val : *first;
            ++first;
            ++result;
        }
    }

    // replace_copy_if

    template <class InputIterator, class OutputIterator, class UnaryPredicate, class T>
    OutputIterator replace_copy_if(InputIterator first, InputIterator last, 
            OutputIterator result, UnaryPredicate pred, const T& val) {
        while (first != last) {
            *result = pred(*first) ? val : *first;
            ++first;
            ++result;
        }
        return result;
    }

    // fill

    template <class ForwardIterator, class T>
    void fill(ForwardIterator first, ForwardIterator last, const T& val) {
        while (first != last) {
            *first = val;
            ++first;
        }
    }

    // fill_n

    template <class OutputIterator, class Size, class T>
    OutputIterator fill_n(OutputIterator first, Size n, const T& val) {
        while (n > 0) {
            *first = val;
            ++first;
            --n;
        }
        return first;
    }

    // generate

    template <class ForwardIterator, class Generator>
    void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
        while (first != last) {
            *first = gen();
            ++first;
        }
    }

    // generate_n

    template <class OutputIterator, class Size, class Generator>
    OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
        while (n > 0) {
            *first = gen();
            ++first;
            --n;
        }
        return first;
    }
    
    // remove

    template <class ForwardIterator, class T>
    ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& val) {
        ForwardIterator result = first;
        while (first != last) {
            if (*first != val) {
                *result = *first;
                ++result;
            }
            ++first;
        }
        return result;
    }

    // remove_if

    template <class ForwardIterator, class UnaryPredicate>
    ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred) {
        ForwardIterator result = first;
        while (first != last) {
            if (!pred(*first)) {
                *result = *first;
                ++result;
            }
            ++first;
        }
        return result; 
    }

    // remove_copy
    
    template <class InputIterator, class OutputIterator, class T>
    OutputIterator remove_copy(InputIterator first, InputIterator last, 
            OutputIterator result, const T& val) {
        while (first != last) {
            if (*first != val) {
                *result = *first;
                ++result;
            }
            ++first;
        }
        return result;
    }

    // remove_copy_if

    template <class InputIterator, class OutputIterator, class BinaryPredicate>
    OutputIterator remove_copy_if(InputIterator first, InputIterator last, 
            OutputIterator result, BinaryPredicate pred) {
        while (first != last) {
            if (!pred(*first)) {
                *result = *first;
                ++result;
            }
            ++first;
        }
        return result;
    }

    // unique

    template <class ForwardIterator>
    ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
        if (first != last) {
            ForwardIterator result = first;
            ++first;
            while (first != last) {
                if (*first != *result) {
                    ++result;
                    *result = *first;
                }
                ++first;
            }
            return ++result;
        }
        return last;
    }

    template <class ForwardIterator, class BinaryPredicate>
    ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate pred) {
        if (first != last) {
            ForwardIterator result = first;
            ++first;
            while (first != last) {
                if (!pred(*first, *result)) {
                    ++result;
                    *result = *first;
                }
                ++first;
            }
            return ++result;
        }
        return last;
    }

    // unique_copy

    template <class InputIterator, class OutputIterator>
    OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result) {
        *result = *first;
        ++first;
        while (first != last) {
            typename iterator_traits<InputIterator>::value_type val = *first;
            if (val != *result) {
                ++result;
                *result = val;
            }
            ++first;
        }
        return result;
    }

    template <class InputIterator, class OutputIterator, class BinaryPredicate>
    OutputIterator unique_copy(InputIterator first, InputIterator last, 
            OutputIterator result, BinaryPredicate pred) {
        *result = *first;
        ++first;
        while (first != last) {
            typename iterator_traits<InputIterator>::value_type val = *first;
            if (!pred(val, *result)) {
                ++result;
                *result = val;
            }
            ++first;
        }
        return result;
    }

    // reverse

    template <class BidirectionalIterator>
    void reverse(BidirectionalIterator first, BidirectionalIterator last) {
        __reverse(first, last, typename iterator_traits<BidirectionalIterator>::iterator_category());
    }

    template <class BidirectionalIterator>
    void __reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
        while (first != last && first != --last) {
            iter_swap(first, last);
            ++first;
        }
    }

    template <class RandomAccessIterator>
    void __reverse(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
        typename iterator_traits<RandomAccessIterator>::difference_type n = (last - first) / 2;
        while (n > 0) {
            --last;
            iter_swap(first, last);
            ++first;
            --n;
        }
    }

    // reverse_copy

    template <class BidirectionalIterator, class OutputIterator>
    OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
        while (first != last) {
            --last;
            *result = *last;
            ++result;
        }
        return result;
    }

    // rotate**

    template <class ForwardIterator>
    void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
        __rotate(first, middle, last, typename iterator_traits<ForwardIterator>::iterator_category());
    }

    template <class ForwardIterator>
    void __rotate(ForwardIterator first, ForwardIterator middle, 
            ForwardIterator last, forward_iterator_tag) {
        ForwardIterator next = middle;
        while (first != last) {
            iter_swap(first, next);
            ++first;
            ++next;
            if (first == middle) {
                if (next == last) {
                    return;
                }
                middle = next;
            } else if (next == last) {
                next = middle;
            }
        }

    }

    template <class BidirectionalIterator>
    void __rotate(BidirectionalIterator first, BidirectionalIterator middle, 
            BidirectionalIterator last, bidirectional_iterator_tag) {
        reverse(first, middle);
        reverse(middle, last);
        reverse(first, last);
    }

    template <class RandomAccessIterator>
    void __rotate(RandomAccessIterator first, RandomAccessIterator middle, 
            RandomAccessIterator last, random_access_iterator_tag);

    // rotate_copy

    template <class ForwardIterator, class OutputIterator>
    OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, 
            ForwardIterator last, OutputIterator result) {
        result = copy(middle, last, result);
        return copy(first, middle, result);
    }

    // random_shuffle**

    template <class RandomAccessIterator>
    void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type distance;
        distance n = last - first;
        for (int i = 0; i != n; ++i) {
            swap(first[i], first[rand() % (i + 1)]);
        }
    }

    template <class RandomAccessIterator, class RandomNumberGenerator>
    void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator& gen) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type distance;
        distance n = last - first;
        for (int i = 0; i != n; ++i) {
            swap(first[i], first[gen(i + 1)]);
        }
    }

    /*
     * Partitions 
     */

    // is_partitioned
    
    template <class InputIterator, class UnaryPredicate>
    bool is_partitioned(InputIterator first, InputIterator last, UnaryPredicate pred) {
        while (first != last && pred(*first)) {
            ++first;
        }
        while (first != last) {
            if (pred(*first)) {
                return false;
            }
            ++first;
        }
        return true;
    }

    // partitioned**

    template <class ForwardIterator, class UnaryPredicate>
    ForwardIterator partition(ForwardIterator first, ForwardIterator last, UnaryPredicate pred) {
        return __partition(first, last, pred, typename iterator_traits<ForwardIterator>::iterator_category());
    }

    template <class ForwardIterator, class UnaryPredicate>
    ForwardIterator __partition(ForwardIterator first, ForwardIterator last, 
            UnaryPredicate pred, forward_iterator_tag) {
        while (pred(*first)) {
            ++first;
            if (first == last) {
                return last;
            }
        }
        ForwardIterator next = first;
        while (++next != last) {
            while (!pred(*next)) {
                ++next;
                if (next == last) {
                    return first;
                }
            }
            iter_swap(next, first);
            ++first;
        }
        return first;
    }

    template <class BidirectionalIterator, class UnaryPredicate>
    BidirectionalIterator __partition(BidirectionalIterator first, BidirectionalIterator last, 
            UnaryPredicate pred, bidirectional_iterator_tag) {
        while (first != last) {
            while (pred(*first)) {
                ++first;
                if (first == last) {
                    return first;
                }
            }
            do {
                --last;
                if (first == last) {
                    return first;
                }
            } while (!pred(*last));
            iter_swap(first, last);
            ++first;
        }
        return first;
    }

    // stable_partition**

    template <class BidirectionalIterator, class UnaryPredicate>
    BidirectionalIterator stable_partition(BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate pred) {
        while (pred(*first)) {
            ++first;
            if (first == last) {
                return first;
            }
        }
        BidirectionalIterator tmp = first;
        while (first != last) {
            while (!pred(*tmp)) {
                ++tmp;
                if (tmp == last) {
                    return first;
                }
            }
            BidirectionalIterator middle = tmp;
            while (tmp != last && pred(*tmp)) {
                ++tmp;
            }
            rotate(first, middle, tmp);
            first = middle;
        }
        return first;
    }

}


#endif
