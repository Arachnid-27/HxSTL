#ifndef _ITERATOR_H_
#define _ITERATOR_H_


#include <stddef.h>


namespace HxSTL {

    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag: public input_iterator_tag {};
    struct bidirectional_iterator_tag: public forward_iterator_tag {};
    struct random_access_iterator_tag: public bidirectional_iterator_tag {};
    
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

    template <class Iterator>
    class reverse_iterator {
    public:
        typedef typename iterator_traits<Iterator>::value_type          value_type;
        typedef typename iterator_traits<Iterator>::difference_type     difference_type;
        typedef typename iterator_traits<Iterator>::pointer             pointer;
        typedef typename iterator_traits<Iterator>::reference           reference;
        typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
        typedef Iterator                                                iterator_type;
    protected:
        Iterator _it;
    public:
        reverse_iterator() {}

        explicit reverse_iterator(Iterator it): _it(it) {}

        reverse_iterator(const reverse_iterator& other): _it(other._it) {}

        template <class Iter>
        reverse_iterator(const reverse_iterator<Iter>& other): _it(other._it) {}

        Iterator base() const noexcept { return _it; }

        reference operator*() const {
            Iterator it = _it;
            return *--it;
        }

        pointer operator->() const { return &(operator*()); }

        reverse_iterator& operator++() {
            --_it;
            return *this;
        }

        reverse_iterator operator++(int) {
            auto it = *this;
            --_it;
            return it;
        }

        reverse_iterator& operator--() {
            ++_it;
            return *this;
        }

        reverse_iterator operator--(int) {
            auto it = *this;
            ++_it;
            return it;
        }

        reverse_iterator operator+(difference_type n) const {
            return reverse_iterator(_it - n);
        }

        reverse_iterator& operator+=(difference_type n) {
            _it -= n;
            return *this;
        }

        reverse_iterator operator-(difference_type n) const {
            return reverse_iterator(_it + n);
        }

        reverse_iterator& operator-=(difference_type n) {
            _it += n;
            return *this;
        }

        reference operator[](difference_type n) const { return *(*this + n); }
    };

    template <class Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) noexcept {
        return lhs.base() == rhs.base();
    }

    template <class Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) noexcept {
        return !(lhs == rhs);
    }

    template <class Iterator>
    bool operator< (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) noexcept {
        return lhs.base() < rhs.base();
    }

    template <class Iterator>
    bool operator> (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) noexcept {
        return rhs < lhs;
    }

    template <class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) noexcept {
        return !(rhs < lhs);
    }

    template <class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) noexcept {
        return !(lhs < rhs);
    }

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
