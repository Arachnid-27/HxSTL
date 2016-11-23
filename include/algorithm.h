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

    template <class InputIt, class UnaryPredicate>
    bool all_of(InputIt first, InputIt last, UnaryPredicate pred) {
        while (first != last) {
            if (!pred(*first)) {
                return false;
            }
            ++first;
        }
        return true;
    }

    // any_of

    template <class InputIt, class UnaryPredicate>
    bool any_of(InputIt first, InputIt last, UnaryPredicate pred) {
        while (first != last) {
            if (pred(*first)) {
                return true;
            }
            ++first;
        }
        return false;
    }

    // none_of

    template <class InputIt, class UnaryPredicate>
    bool none_of(InputIt first, InputIt last, UnaryPredicate pred) {
        while (first != last) {
            if (pred(*first)) {
                return false;
            }
            ++first;
        }
        return true;
    }

    // for_each

    template <class InputIt, class Function>
    Function for_each(InputIt first, InputIt last, Function fn) {
        while (first != last) {
            fn(*first);
            ++first;
        }
        return fn;
    }

    // find

    template <class InputIt, class T>
    InputIt find(InputIt first, InputIt last, const T& val) {
        while (first != last && *first != val) {
            ++first;
        }
        return first;
    }

    // find_if

    template <class InputIt, class UnaryPredicate>
    InputIt find_if(InputIt first, InputIt last, UnaryPredicate pred) {
        while (first != last && !pred(*first)) {
            ++first;
        }
        return first;
    }

    // find_if_not

    template <class InputIt, class UnaryPredicate>
    InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate pred) {
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

    template <class BidirIt1, class BidirIt2>
    BidirIt1 __find_end(BidirIt1 first1, BidirIt1 last1, 
            BidirIt2 first2, BidirIt2 last2, 
            bidirectional_iterator_tag, bidirectional_iterator_tag) {
        BidirIt1 tmp = last1;
        while (first1 != last1) {
            BidirIt1 it1 = last1;
            BidirIt2 it2 = last2;
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

    template <class BidirIt1, class BidirIt2, class BinaryPredicate>
    BidirIt1 __find_end(BidirIt1 first1, BidirIt1 last1, 
            BidirIt2 first2, BidirIt2 last2, BinaryPredicate pred,
            bidirectional_iterator_tag, bidirectional_iterator_tag) {
        BidirIt1 tmp = last1;
        while (first1 != last1) {
            BidirIt1 it1 = last1;
            BidirIt2 it2 = last2;
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

    template <class InputIt, class ForwardIterator>
    InputIt find_first_of(InputIt first1, InputIt last1, 
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

    template <class InputIt, class ForwardIterator, class BinaryPredicate>
    InputIt find_first_of(InputIt first1, InputIt last1, 
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

    template <class InputIt, class T>
    typename iterator_traits<InputIt>::difference_type
    count(InputIt first, InputIt last, const T& val) {
        typename iterator_traits<InputIt>::difference_type result = 0;
        while (first != last) {
            if (*first == val) {
                ++result;
            }
            ++first;
        }
        return result;
    }

    // count_if

    template <class InputIt, class UnaryPredicate>
    typename iterator_traits<InputIt>::difference_type
    count_if(InputIt first, InputIt last, UnaryPredicate pred) {
        typename iterator_traits<InputIt>::difference_type result = 0;
        while (first != last) {
            if (pred(*first)) {
                ++result;
            }
            ++first;
        }
        return result;
    }

    // mismatch

    template <class InputIt1, class InputIt2>
    pair<InputIt1, InputIt2>
    mismatch(InputIt1 first1, InputIt1 last1, 
            InputIt2 first2) {
        while (first1 != last1 && *first1 == *first2) {
            ++first1;
            ++first2;
        }
        return make_pair(first1, first2);
    }

    template <class InputIt1, class InputIt2, class BinaryPredicate>
    pair<InputIt1, InputIt2>
    mismatch(InputIt1 first1, InputIt1 last1, 
            InputIt2 first2, BinaryPredicate pred) {
        while (first1 != last1 && pred(*first1, *first2)) {
            ++first1;
            ++first2;
        }
        return make_pair(first1, first2);
    }

    // equal

    template <class InputIt1, class InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1, 
            InputIt2 first2) {
        while (first1 != last1) {
            if (*first1 != *first2) {
                return false;
            }
            ++first1;
            ++first2;
        }
        return true;
    }

    template <class InputIt1, class InputIt2, class BinaryPredicate>
    bool equal(InputIt1 first1, InputIt1 last1, 
            InputIt2 first2, BinaryPredicate pred) {
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

    template <class InputIt, class OutputIterator>
    struct __copy_dispath {
        OutputIterator operator()(InputIt first, InputIt last, OutputIterator result) {
            return __copy(first, last, result, 
                    typename iterator_traits<InputIt>::iterator_category());
        }
    };

    template <class T>
    struct __copy_dispath<T*, T*> {
        T* operator()(T* first, T* last, T* result) {
            return __copy_ptr(first, last, result, typename is_pod<T>::type());
        }
    };

    template <class T>
    struct __copy_dispath<const T*, T*> {
        T* operator()(const T* first, const T* last, T* result) {
            return __copy_ptr(first, last, result, typename is_pod<T>::type());
        }
    };

    template <class InputIt, class OutputIterator>
    inline OutputIterator __copy(InputIt first, InputIt last, 
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
        return __copy_rand(first, last, result);
    }

    template <class InputIt, class OutputIterator>
    OutputIterator copy(InputIt first, InputIt last, 
            OutputIterator result) {
        return __copy_dispath<InputIt, OutputIterator>()(first, last, result);
    }

    // copy_n

    template <class InputIt, class Size, class OutputIterator>
    struct __copy_n_dispath {
        OutputIterator operator()(InputIt first, Size n, OutputIterator result) {
            return __copy_n(first, n, result);
        }
    };

    template <class T, class Size>
    struct __copy_n_dispath<T*, Size, T*> {
        T* operator()(T* first, Size n, T* result) {
            return __copy_n_ptr(first, n, result, typename is_pod<T>::type());
        }
    };

    template <class T, class Size>
    struct __copy_n_dispath<const T*, Size, T*> {
        T* operator()(const T* first, Size n, T* result) {
            return __copy_n_ptr(first, n, result, typename is_pod<T>::type());
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

    template <class InputIt, class Size, class OutputIterator>
    inline OutputIterator __copy_n(InputIt first, Size n, OutputIterator result) {
        while (n > 0) {
            *result = *first;
            --n;
            ++result;
            ++first;
        }
        return result;
    }

    template <class InputIt, class Size, class OutputIterator>
    OutputIterator copy_n(InputIt first, Size n, OutputIterator result) {
        return __copy_n_dispath<InputIt, Size, OutputIterator>()(first, n, result);
    }

    // copy_if

    template <class InputIt, class OutputIterator, class UnaryPredicate>
    OutputIterator copy_if(InputIt first, InputIt last, 
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

    template <class BidirIt1, class BidirIt2>
    struct __copy_backward_dispath {
        BidirIt2 operator()(BidirIt1 first, BidirIt1 last, BidirIt2 result) {
            return __copy_backward(first, last, result, 
                    typename iterator_traits<BidirIt1>::iterator_category());
        }
    };

    template <class T>
    struct __copy_backward_dispath<T*, T*> {
        T* operator()(T* first, T* last, T* result) {
            return __copy_backward_ptr(first, last, result, typename is_pod<T>::type());
        }
    };

    template <class T>
    struct __copy_backward_dispath<const T*, T*> {
        T* operator()(const T* first, const T* last, T* result) {
            return __copy_backward_ptr(first, last, result, typename is_pod<T>::type());
        }
    };

    template <class BidirIt1, class BidirIt2>
    inline BidirIt2 __copy_backward(BidirIt1 first, BidirIt1 last, 
            BidirIt2 result, bidirectional_iterator_tag) {
        while (first != last) {
            *(--result) = *(--last);
        }
        return result;
    }

    template <class RandomAccessIterator, class BidirIt>
    inline BidirIt __copy_backward(RandomAccessIterator first, RandomAccessIterator last, 
            BidirIt result, random_access_iterator_tag) {
        return __copy_backward_rand(first, last, result);
    }

    template <class RandomAccessIterator, class BidirIt>
    inline BidirIt __copy_backward_rand(RandomAccessIterator first, RandomAccessIterator last, 
            BidirIt result) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type distance;
        for (distance n = last - first; n > 0; --n) {
            *(--result) = *(--last);
        }
        return result;
    }

    template <class T>
    inline T* __copy_backward_ptr(const T* first, const T* last, T* result, true_type) {
        memmove(result - (last - first), first, sizeof(T) * (last - first));
        return result - (last - first);
    }
    
    template <class T>
    inline T* __copy_backward_ptr(const T* first, const T* last, T* result, false_type) {
        return __copy_backward_rand(first, last, result);
    }

    template <class BidirIt1, class BidirIt2>
    BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 result) {
        return __copy_backward_dispath<BidirIt1, BidirIt2>()(first, last, result);
    }

    // move
    
    template <class InputIt, class OutputIterator>
    OutputIterator move(InputIt first, InputIt last, OutputIterator result) {
        while (first != last) {
            *result = HxSTL::move(*first);
            ++first;
            ++result;
        }
        return result;
    }

    // move_backward

    template <class BidirIt1, class BidirIt2>
    BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 result) {
        while (first != last) {
            *(--result) = HxSTL::move(*(--last));
        }
        return result;
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

    template <class InputIt, class OutputIterator, class UnaryPredicate>
    OutputIterator transform(InputIt first1, InputIt last1, 
            OutputIterator result, UnaryPredicate op) {
        while (first1 != last1) {
            *result = op(first1);
            ++first1;
            ++result;
        }
        return result;
    }

    template <class InputIt1, class InputIt2, class OutputIterator, class BinaryPredicate>
    OutputIterator transform(InputIt1 first1, InputIt1 last1, 
            InputIt2 first2, OutputIterator result, BinaryPredicate op) {
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

    template <class InputIt, class OutputIterator, class UnaryPredicate>
    void replace_if(InputIt first, InputIt last, 
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
    
    template <class InputIt, class OutputIterator, class T>
    void replace_copy(InputIt first, InputIt last, 
            OutputIterator result, const T& old_val, const T& new_val) {
        while (first != last) {
            *result = *first == old_val ? new_val : *first;
            ++first;
            ++result;
        }
    }

    // replace_copy_if

    template <class InputIt, class OutputIterator, class UnaryPredicate, class T>
    OutputIterator replace_copy_if(InputIt first, InputIt last, 
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
    
    template <class InputIt, class OutputIterator, class T>
    OutputIterator remove_copy(InputIt first, InputIt last, 
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

    template <class InputIt, class OutputIterator, class BinaryPredicate>
    OutputIterator remove_copy_if(InputIt first, InputIt last, 
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

    template <class InputIt, class OutputIterator>
    OutputIterator unique_copy(InputIt first, InputIt last, OutputIterator result) {
        *result = *first;
        ++first;
        while (first != last) {
            typename iterator_traits<InputIt>::value_type val = *first;
            if (val != *result) {
                ++result;
                *result = val;
            }
            ++first;
        }
        return result;
    }

    template <class InputIt, class OutputIterator, class BinaryPredicate>
    OutputIterator unique_copy(InputIt first, InputIt last, 
            OutputIterator result, BinaryPredicate pred) {
        *result = *first;
        ++first;
        while (first != last) {
            typename iterator_traits<InputIt>::value_type val = *first;
            if (!pred(val, *result)) {
                ++result;
                *result = val;
            }
            ++first;
        }
        return result;
    }

    // reverse

    template <class BidirIt>
    void reverse(BidirIt first, BidirIt last) {
        __reverse(first, last, typename iterator_traits<BidirIt>::iterator_category());
    }

    template <class BidirIt>
    void __reverse(BidirIt first, BidirIt last, bidirectional_iterator_tag) {
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

    template <class BidirIt, class OutputIterator>
    OutputIterator reverse_copy(BidirIt first, BidirIt last, OutputIterator result) {
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

    template <class BidirIt>
    void __rotate(BidirIt first, BidirIt middle, 
            BidirIt last, bidirectional_iterator_tag) {
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
    
    template <class InputIt, class UnaryPredicate>
    bool is_partitioned(InputIt first, InputIt last, UnaryPredicate pred) {
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

    template <class BidirIt, class UnaryPredicate>
    BidirIt __partition(BidirIt first, BidirIt last, 
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

    template <class BidirIt, class UnaryPredicate>
    BidirIt stable_partition(BidirIt first, BidirIt last, UnaryPredicate pred) {
        while (pred(*first)) {
            ++first;
            if (first == last) {
                return first;
            }
        }
        BidirIt tmp = first;
        while (first != last) {
            while (!pred(*tmp)) {
                ++tmp;
                if (tmp == last) {
                    return first;
                }
            }
            BidirIt middle = tmp;
            while (tmp != last && pred(*tmp)) {
                ++tmp;
            }
            rotate(first, middle, tmp);
            first = middle;
        }
        return first;
    }

    // partition_copy

    template <class InputIt, class OutputIterator1, class OutputIterator2, class UnaryPredicate>
    pair<OutputIterator1, OutputIterator2> partition_copy(InputIt first, InputIt last, 
            OutputIterator1 result_true, OutputIterator2 result_false, UnaryPredicate pred) {
        while (first != last) {
            typename iterator_traits<InputIt>::value_type val = *first;
            if (pred(val)) {
                *result_true = val;
                ++result_true;
            } else {
                *result_false = val;
                ++result_false;
            }
            ++first;
        }
        return make_pair(result_true, result_false);
    }

    // partition_point

    template <class ForwardIterator, class UnaryPredicate>
    ForwardIterator partition_point(ForwardIterator first, ForwardIterator last, UnaryPredicate pred) {
    }

    /**
     * Heap
     */

    template <class RandomAccessIterator>
    void __adjust_heap(RandomAccessIterator first, RandomAccessIterator last) {
        typename iterator_traits<RandomAccessIterator>::difference_type n = last - first - 1;
        typename iterator_traits<RandomAccessIterator>::difference_type parent = (n - 1) / 2;
        typename iterator_traits<RandomAccessIterator>::value_type val = *(first + n);
        while (n > 0 && *(first + parent) < val) {
            *(first + n) = *(first + parent);
            n = parent;
            parent = (parent - 1) / 2;
        }
        *(first + n) = val;
    }

    template <class RandomAccessIterator, class Compare>
    void __adjust_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        typename iterator_traits<RandomAccessIterator>::difference_type n = last - first - 1;
        typename iterator_traits<RandomAccessIterator>::difference_type parent = (n - 1) / 2;
        typename iterator_traits<RandomAccessIterator>::value_type val = *(first + n);
        while (n > 0 && comp(*(first + parent) ,val)) {
            *(first + n) = *(first + parent);
            n = parent;
            parent = (parent - 1) / 2;
        }
        *(first + n) = val;
    }

    template <class RandomAccessIterator, class Distance>
    void __maintain_heap(RandomAccessIterator first, Distance index, Distance max) {
        RandomAccessIterator cur = first + index;
        RandomAccessIterator left, right;
        typename iterator_traits<RandomAccessIterator>::value_type val = *cur;
        while (index * 2 + 1 < max) {
            left = first + (index * 2 + 1);
            if (index * 2 + 2 < max) {
                right = first + (index * 2 + 2);
                if (*left < val) {
                    if (val < *right) {
                        *cur = *right;
                        cur = right;
                        index = index * 2 + 2;
                    } else {
                        break;
                    }
                } else if (*left < *right) {
                    *cur = *right;
                    cur = right;
                    index = index * 2 + 2;
                } else {
                    *cur = *left;
                    cur = left;
                    index = index * 2 + 1;
                }
            } else if (val < *left) {
                *cur = *left;
                cur = left;
                break;
            } else {
                break;
            }
        }
        *cur = val;
    }

    template <class RandomAccessIterator, class Distance, class Compare>
    void __maintain_heap(RandomAccessIterator first, Distance index, Distance max, Compare comp) {
        RandomAccessIterator cur = first + index;
        RandomAccessIterator left, right;
        typename iterator_traits<RandomAccessIterator>::value_type val = *cur;
        while (index * 2 + 1 < max) {
            left = first + (index * 2 + 1);
            if (index * 2 + 2 < max) {
                right = first + (index * 2 + 2);
                if (comp(*left, val)) {
                    if (comp(val, *right)) {
                        *cur = *right;
                        cur = right;
                        index = index * 2 + 2;
                    } else {
                        break;
                    }
                } else if (comp(*left, *right)) {
                    *cur = *right;
                    cur = right;
                    index = index * 2 + 2;
                } else {
                    *cur = *left;
                    cur = left;
                    index = index * 2 + 1;
                }
            } else if (comp(val, *left)) {
                *cur = *left;
                cur = left;
                break;
            } else {
                break;
            }
        }
        *cur = val;
    }

    // push_heap

    template <class RandomAccessIterator>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
        __adjust_heap(first, last);
    }

    template <class RandomAccessIterator, class Compare>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        __adjust_heap(first, last, comp);
    }

    // pop_heap

    template <class RandomAccessIterator>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
        iter_swap(first, last - 1);
        __maintain_heap(first, static_cast<difference_type>(0), last - first - 1);
    }

    template <class RandomAccessIterator, class Compare>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
        iter_swap(first, last - 1);
        __maintain_heap(first, static_cast<difference_type>(0), last - first - 1, comp);
    }

    // make_heap

    template <class RandomAccessIterator>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
        if (last - first > 1) {
            difference_type max = last - first;
            difference_type index = (max - 2) / 2;
            while (index >= 0) {
                __maintain_heap(first, index, max);
                --index;
            }
        }
    }

    template <class RandomAccessIterator, class Compare>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
        if (last - first > 1) {
            difference_type max = last - first;
            difference_type index = (max - 2) / 2;
            while (index >= 0) {
                __maintain_heap(first, index, max, comp);
                --index;
            }
        }
    }

    // sort_heap

    template <class RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
        while (last - first > 1) {
            pop_heap(first, last);
            --last;
        }
    }

    template <class RandomAccessIterator, class Compare>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        while (last - first > 1) {
            pop_heap(first, last, comp);
            --last;
        }
    }

    // is_heap

    template <class RandomAccessIterator>
    bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
        typename iterator_traits<RandomAccessIterator>::difference_type n = (last - first - 2) / 2;
        while (n >= 0) {
            if (*(first + n) < *(first + (2 * n + 1)) || *(first + n) < *(first + (2 * n + 2))) {
                return false;
            }
            --n;
        }
        return true;
    }

    template <class RandomAccessIterator, class Compare>
    bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        typename iterator_traits<RandomAccessIterator>::difference_type n = (last - first - 2) / 2;
        while (n >= 0) {
            if (comp(*(first + n), *(first + (2 * n + 1))) || comp(*(first + n), *(first + (2 * n + 2)))) {
                return false;
            }
            --n;
        }
        return true;
    }

    /*
     * Sorting
     */

    const int  __introsort_threshold = 16;

    template <class RandomAccessIterator, class T>
    void __unguarded_linear_insert(RandomAccessIterator last, const T& val) {
        RandomAccessIterator next = last;
        --next;

        while (val < *next) {
            *last = *next;
            last = next;
            --next;
        }

        *last = val;
    }

    template <class RandomAccessIterator, class T, class Compare>
    void __unguarded_linear_insert(RandomAccessIterator last, const T& val, Compare comp) {
        RandomAccessIterator next = last;
        --next;

        while (comp(val, *next)) {
            *last = *next;
            last = next;
            --next;
        }

        *last = val;
    }

    template <class RandomAccessIterator>
    void __linear_insert(RandomAccessIterator first, RandomAccessIterator last) {
        typename iterator_traits<RandomAccessIterator>::value_type val = *last;
        if (val < *first) {     // 边界最小
            copy_backward(first, last, last + 1);
            *first = val;
        } else {
            __unguarded_linear_insert(last, val);
        }
    }

    template <class RandomAccessIterator, class Compare>
    void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        typename iterator_traits<RandomAccessIterator>::value_type val = *last;
        if (comp(val, *first)) {
            copy_backward(first, last, last + 1);
            *first = val;
        } else {
            __unguarded_linear_insert(last, val, comp);
        }
    }

    template <class RandomAccessIterator>
    void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (first != last) {
            for (RandomAccessIterator it = first + 1; it != last; ++it) {
                __linear_insert(first, it);
            }
        }
    }

    template <class RandomAccessIterator, class Compare>
    void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        if (first != last) {
            for (RandomAccessIterator it = first + 1; it != last; ++it) {
                __linear_insert(first, it, comp);
            }
        }
    }

    template <class RandomAccessIterator>
    void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
        for (RandomAccessIterator it = first; it != last; ++it) {
            __unguarded_linear_insert(it, *it);
        }
    }

    template <class RandomAccessIterator, class Compare>
    void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        for (RandomAccessIterator it = first; it != last; ++it) {
            __unguarded_linear_insert(it, *it, comp);
        }
    }

    template <class RandomAccessIterator>
    void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (last - first > __introsort_threshold) {
            __insertion_sort(first, first + __introsort_threshold);
            __unguarded_insertion_sort(first + __introsort_threshold, last);
        } else {
            __insertion_sort(first, last);
        }
    }

    template <class RandomAccessIterator, class Compare>
    void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        if (last - first > __introsort_threshold) {
            __insertion_sort(first, first + __introsort_threshold, comp);
            __unguarded_insertion_sort(first + __introsort_threshold, last, comp);
        } else {
            __insertion_sort(first, last, comp);
        }
    }

    template <class Size>
    Size __lg(Size n) {
        Size k = 0;
        while (n != 0) {
            ++k;
            n >>= 1;
        }
        return k;
    }

    template <class RandomAccessIterator, class T>
    RandomAccessIterator __pivot_parition(RandomAccessIterator first, RandomAccessIterator last, const T& pivot) {
        while (true) {
            while (*first < pivot) {
                ++first;
            }
            --last;
            while (pivot < *last) {
                --last;
            }
            if (first < last) {
                iter_swap(first, last);
                ++first;
            } else {
                return first;
            }
        }
    }

    template <class RandomAccessIterator, class T, class Compare>
    RandomAccessIterator __pivot_parition(RandomAccessIterator first, RandomAccessIterator last, 
            const T& pivot, Compare comp) {
        while (true) {
            while (comp(*first, pivot)) {
                ++first;
            }
            --last;
            while (comp(pivot, *last)) {
                --last;
            }
            if (first < last) {
                iter_swap(first, last);
                ++first;
            } else {
                return first;
            }
        }
    }

    template <class T>
    const T& __median(const T& a, const T& b, const T& c) {
        if (a < b) {
            if (b < c) {
                return b;
            } else if (a < c) {
                return c;
            } else {
                return a;
            }
        } else if (a < c) {
            return a;
        } else if (b < c) {
            return c;
        } else {
            return b;
        }
    }

    template <class T, class Compare>
    const T& __median(const T& a, const T& b, const T& c, Compare comp) {
        if (comp(a, b)) {
            if (comp(b, c)) {
                return b;
            } else if (comp(a, c)) {
                return c;
            } else {
                return a;
            }
        } else if (comp(a, c)) {
            return a;
        } else if (comp(b, c)) {
            return c;
        } else {
            return b;
        }
    }

    template <class RandomAccessIterator, class Size>
    void __introsort(RandomAccessIterator first, RandomAccessIterator last, Size limit) {
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
        while (last - first > __introsort_threshold) {
            if (limit == 0) {
                make_heap(first, last);
                sort_heap(first, last);
                return;
            }
            --limit;

            value_type pivot = __median(*first, *(first + (last - first) / 2), *(last - 1));
            RandomAccessIterator middle = __pivot_parition(first, last, pivot);

            __introsort(middle, last, limit);
            last = middle;  // 左侧循环代替递归
        }
    }

    template <class RandomAccessIterator, class Size, class Compare>
    void __introsort(RandomAccessIterator first, RandomAccessIterator last, Size limit, Compare comp) {
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
        while (last - first > __introsort_threshold) {
            if (limit == 0) {
                make_heap(first, last, comp);
                sort_heap(first, last, comp);
                return;
            }
            --limit;

            value_type pivot = __median(*first, *(first + (last - first) / 2), *(last - 1), comp);
            RandomAccessIterator middle = __pivot_parition(first, last, pivot, comp);

            __introsort(middle, last, limit, comp);
            last = middle;
        }
    }

    // sort

    template <class RandomAccessIterator>
    void sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (first != last) {
            __introsort(first, last, __lg(last - first) * 2);
            __final_insertion_sort(first, last);
        }
    }

    template <class RandomAccessIterator, class Compare>
    void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        if (first != last) {
            __introsort(first, last, __lg(last - first) * 2, comp);
            __final_insertion_sort(first, last, comp);
        }
    }

    // partial_sort

    template <class RandomAccessIterator>
    void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
        make_heap(first, middle);
        for (RandomAccessIterator it = middle; it != last; ++it) {
            if (*it < *first) {
                iter_swap(first, it);
                __maintain_heap(first, static_cast<difference_type>(0), middle - first);
            }
        }
        sort_heap(first, middle);
    }

    template <class RandomAccessIterator, class Compare>
    void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, 
            RandomAccessIterator last, Compare comp) {
        typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
        make_heap(first, middle);
        for (RandomAccessIterator it = middle; it != last; ++it) {
            if (comp(*it, *first)) {
                iter_swap(first, it);
                __maintain_heap(first, static_cast<difference_type>(0), middle - first);
            }
        }
        sort_heap(first, middle);
    }

    /**
     * Min/max
     */

    // min

    template <class T>
    const T& min(const T& a, const T& b) {
        return b < a ? b : a;
    }

    template <class T, class Compare>
    const T& min(const T& a, const T& b, Compare comp) {
        return comp(b, a) ? a : b;
    }

    // max

    template <class T>
    const T& max(const T& a, const T& b) {
        return b < a ? a : b;
    }

    template <class T, class Compare>
    const T& max(const T& a, const T& b, Compare comp) {
        return comp(a, b) ? a : b;
    }

    // min_element

    template <class ForwardIterator>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
        ForwardIterator result = first;
        while (first != last) {
            if (*first < *result) {
                result = first;
            }
        }
        return result;
    }

    template <class ForwardIterator, class Compare>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
        ForwardIterator result = first;
        while (first != last) {
            if (comp(*first, *result)) {
                result = first;
            }
        }
        return result;
    }

    // max_element

    template <class ForwardIterator>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
        ForwardIterator result = first;
        while (first != last) {
            if (*result < *first) {
                result = first;
            }
        }
        return result;
    }

    template <class ForwardIterator, class Compare>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
        ForwardIterator result = first;
        while (first != last) {
            if (comp(*result, *first)) {
                result = first;
            }
        }
        return result;
    }

    // lexicographical_compare

    template <class InputIt1, class InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                return true;
            }
            if (*first2 < *first1) {
                return false;
            }
            ++first1;
            ++first2;
        }
        return (first1 == last1) && (first2 != last2);
    }

    template <class InputIt1, class InputIt2, class Compare>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first1, *first2)) {
                return true;
            }
            if (comp(*first2, *first1)) {
                return false;
            }
            ++first1;
            ++first2;
        }
        return (first1 == last1) && (first2 != last2);
    }
}


#endif
