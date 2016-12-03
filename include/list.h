#ifndef _LIST_H_
#define _LIST_H_


#include "allocator.h"
#include "uninitialized.h"
#include "utility.h"


namespace HxSTL {

    template <class T>
    struct __list_node {
        T data;
        __list_node<T>* prev;
        __list_node<T>* next;
    };

    template <class T, class Alloc = allocator<__list_node<T>>>
    class list;

    template <class T>
    class __list_const_iterator;

    template <class T>
    class __list_iterator {
    public:
        typedef bidirectional_iterator_tag          iterator_category;
        typedef T                                   value_type;
        typedef T*                                  pointer;
        typedef T&                                  reference;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;
        typedef __list_node<value_type>*              link_type;
    protected:
        link_type _node;
    public:
        __list_iterator() {}

        __list_iterator(link_type node): _node(node) {}

        __list_iterator(const __list_iterator& other): _node(other._node) {}
        
        reference operator*() const { return _node -> data; }

        pointer operator->() const { return &(operator*()); }

        __list_iterator& operator++() {
            _node = _node -> next;
            return *this;
        }

        __list_iterator& operator++(int) {
            __list_iterator& it = *this;
            ++*this;
            return it;
        }

        __list_iterator& operator--() {
            _node = _node -> prev;
            return *this;
        }

        __list_iterator& operator--(int) {
            __list_iterator& it = *this;
            --*this;
            return it;
        }
    public:
        template <class U, class Alloc>
        friend class list;
        friend class __list_const_iterator<T>;
        template <class U>
        friend bool operator==(const __list_iterator<U>& lhs, const __list_iterator<U>& rhs);
        template <class U>
        friend bool operator!=(const __list_iterator<U>& lhs, const __list_iterator<U>& rhs);
    };

    template <class T>
    bool operator==(const __list_iterator<T>& lhs, const __list_iterator<T>& rhs) {
        return lhs._node == rhs._node;
    }

    template <class T>
    bool operator!=(const __list_iterator<T>& lhs, const __list_iterator<T>& rhs) {
        return lhs._node != rhs._node;
    }

    template <class T>
    class __list_const_iterator {
    public:
        typedef bidirectional_iterator_tag          iterator_category;
        typedef T                                   value_type;
        typedef const T*                            pointer;
        typedef const T&                            reference;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;
        typedef __list_node<value_type>*              link_type;
    protected:
        link_type _node;
    public:
        __list_const_iterator() {}

        __list_const_iterator(link_type node): _node(node) {}

        __list_const_iterator(const __list_const_iterator& other): _node(other._node) {}

        __list_const_iterator(const __list_iterator<T>& other): _node(other._node) {}
        
        reference operator*() const { return _node -> data; }

        pointer operator->() const { return &(operator*()); }

        __list_const_iterator& operator++() {
            _node = _node -> next;
            return *this;
        }

        __list_const_iterator& operator++(int) {
            __list_const_iterator& it = *this;
            ++*this;
            return it;
        }

        __list_const_iterator& operator--() {
            _node = _node -> prev;
            return *this;
        }

        __list_const_iterator& operator--(int) {
            __list_const_iterator& it = *this;
            --*this;
            return it;
        }
    public:
        template <class U, class Alloc>
        friend class list;
        template <class U>
        friend bool operator==(const __list_const_iterator<U>& lhs, const __list_const_iterator<U>& rhs);
        template <class U>
        friend bool operator!=(const __list_const_iterator<U>& lhs, const __list_const_iterator<U>& rhs);
    };

    template <class T>
    bool operator==(const __list_const_iterator<T>& lhs, const __list_const_iterator<T>& rhs) {
        return lhs._node == rhs._node;
    }

    template <class T>
    bool operator!=(const __list_const_iterator<T>& lhs, const __list_const_iterator<T>& rhs) {
        return lhs._node != rhs._node;
    }

    template <class T, class Alloc>
    class list {
    public:
        typedef T                                           value_type;
        typedef Alloc                                       allocator_type;
        typedef value_type&                                 reference;
        typedef const value_type&                           const_reference;
        typedef value_type*                                 pointer;
        typedef const value_type*                           const_pointer;
        typedef __list_iterator<T>                          iterator;
        typedef __list_const_iterator<T>                    const_iterator;
    //  typedef reverse_iterator<iterator>                  reverse_iterator;
    //  typedef reverse_iterator<const_iterator>            const_reverse_iterator;
        typedef ptrdiff_t                                   difference_type;
        typedef size_t                                      size_type;
        typedef typename iterator::link_type                link_type;
    protected:
        link_type _node;
        allocator_type _alloc;
    protected:
        template <class InputIt>
        void initialize_aux(InputIt first, InputIt last, HxSTL::false_type);
        void initialize_aux(size_type count, const T& value, HxSTL::true_type);
        template <class U>
        const_iterator insert_aux(const_iterator pos, U&& value);
        template <class InputIt>
        const_iterator insert_aux(const_iterator pos, InputIt first, InputIt last, HxSTL::false_type);
        const_iterator insert_aux(const_iterator pos, size_type count, const T& value, HxSTL::true_type);
        template <class... Args>
        iterator emplace_aux(const_iterator pos, Args&&... args);
        template <class InputIt>
        void assign_aux(InputIt first, InputIt last, HxSTL::false_type);
        void assign_aux(size_type n, const T& value, HxSTL::true_type);
        const_iterator erase_aux(const_iterator pos);
        const_iterator erase_aux(const_iterator first, const_iterator last);
        link_type get_node() { return _alloc.allocate(1); }
        template <class... Args>
        link_type create_node(Args&&... args);
        void destroy_node(link_type p);
    public:
        explicit list(const Alloc& alloc = Alloc()): _alloc(alloc) {
            _node = get_node();
            _node -> next = _node;
            _node -> prev = _node;
        }

        explicit list(size_type count, const T& value, const Alloc& alloc = Alloc()): _alloc(alloc) {
            initialize_aux(count, value, HxSTL::true_type());
        }

        explicit list(size_type count, const Alloc& alloc = Alloc()): _alloc(alloc) {
            initialize_aux(count, T(), HxSTL::true_type());
        }

        template <class InputIt>
        list(InputIt first, InputIt last, const Alloc& alloc = Alloc()): _alloc(alloc) {
            initialize_aux(first, last, typename HxSTL::is_integeral<InputIt>::type());
        }

        list(const list& other): _alloc(other._alloc) {
            initialize_aux(other.begin(), other.end(), HxSTL::false_type());
        }

        list(const list& other, const Alloc& alloc): _alloc(alloc) {
            initialize_aux(other.begin(), other.end(), HxSTL::false_type());
        }

        list(list&& other): _node(other._node), _alloc(HxSTL::move(other._alloc)) {
            other._node = nullptr;
        }

        list(list&& other, const Alloc& alloc): _alloc(alloc) {
            other._node = nullptr;
        }

        list(HxSTL::initializer_list<T> init, const Alloc& alloc = Alloc()): _alloc(alloc) {
            initialize_aux(init.begin(), init.end(), HxSTL::false_type());
        }

        ~list() {
            if (_node) {
                clear();
                destroy_node(_node);
            }
        }

        list& operator=(const list& other) {
            list(other).swap(*this);
            return *this;
        }

        list& operator=(list&& other) {
            list(HxSTL::move(other)).swap(*this);
            return *this;
        }

        list& operator=(HxSTL::initializer_list<T> init) {
            list(init).swap(*this);
            return *this;
        }

        void assign(size_type count, const T& value) {
            assign_aux(count, value, HxSTL::true_type());
        }

        template <class InputIt>
        void assign(InputIt first, InputIt last) {
            assign_aux(first, last, typename HxSTL::is_integeral<InputIt>::type());
        }

        allocator_type get_allocator() const { return _alloc; }

        reference front() { return *begin(); }

        const_reference front() const { return *begin(); }

        reference back() { return *(--end()); }

        const_reference back() const { return *(--end()); }

        iterator begin() { return _node -> next; }

        const_iterator begin() const { return _node -> next; }

        const_iterator cbegin() const { return _node -> next; }

        iterator end() { return _node; }

        const_iterator end() const { return _node; }

        const_iterator cend() const { return _node; }

    //  reverse_iterator rbegin();

    //  const_reverse_iterator rbegin() const;

    //  reverse_iterator rend();

    //  const_reverse_iterator rend() const;
    
        bool empty() const { return begin() == end(); }

        size_type size() const { return HxSTL::distance(begin(), end()); }

        size_type max_size() const { return _alloc.max_size(); }
        
        void clear() {
            const_iterator first = begin();
            const_iterator last = end();
            if (first != last) {
                erase_aux(first, last);
            }
        }

        iterator insert(const_iterator pos, const T& value) {
            pos = insert_aux(pos, value);
            return iterator(pos._node);
        }

        iterator insert(const_iterator pos, T&& value) {
            pos = insert_aux(pos, HxSTL::move(value));
            return iterator(pos._node);
        }

        iterator insert(const_iterator pos, size_type count, const T& value) {
            if (count != 0) pos = insert_aux(pos, count, value, HxSTL::true_type());
            return iterator(pos._node);
        }

        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            pos = insert_aux(pos, first, last, typename HxSTL::is_integeral<InputIt>::type());
            return iterator(pos._node);
        }

        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args) {
            return emplace_aux(pos, HxSTL::forward<Args>(args)...);
        }

        iterator erase(const_iterator pos) {
            if (pos != end()) {
                pos = erase_aux(pos);
            }
            return iterator(pos._node);
        }

        iterator erase(const_iterator first, const_iterator last) {
            if (first != last) {
                first = erase_aux(first, last);
            }
            return reinterpret_cast<iterator>(first);
        }

        void push_back(const T& value) { insert_aux(end(), value); }

        void push_back(T&& value) { insert_aux(end(), HxSTL::move(value)); }

        template <class... Args>
        void emplace_back(Args&&... args) { emplace_aux(end(), HxSTL::forward<Args>(args)...); }

        void pop_back() {
            iterator last = end();
            if (last != begin()) {
                erase_aux(--last);
            }
        }

        void push_front(const T& value) { insert_aux(begin(), value); }

        void push_front(T&& value) { insert_aux(begin(), HxSTL::move(value)); }

        template <class... Args>
        void emplace_front(Args&&... args) { emplace_aux(begin(), HxSTL::forward<Args>(args)...); }

        void pop_front() {
            iterator first = begin();
            if (first != end()) {
                erase_aux(first);
            }
        }

        void resize(size_type count) {
            resize(count, T());
        }

        void resize(size_type count, const T& value);

        void swap(list& other) {
            HxSTL::swap(_node, other._node);
        }

        void merge(list& other);

        void merge(list&& other) {
            merge(other);
        }

        template <class Compare>
        void merge(list& other, Compare comp);

        template <class Compare>
        void merge(list&& other, Compare comp) {
            merge(other, comp);
        }

        void splice(const_iterator pos, list& other) {
            splice(pos, other, other.begin(), other.end());
        }

        void splice(const_iterator pos, list&& other) {
            splice(pos, other, other.begin(), other.end());
        }

        void splice(const_iterator pos, list& other, const_iterator it);

        void splice(const_iterator pos, list&& other, const_iterator it) {
            splice(pos, other, it);
        }

        void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

        void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last) {
            splice(pos, other, first, last);
        }

        void remove(const T& value);

        template <class UnaryPredicate>
        void remove_if(UnaryPredicate pred);

        void reverse();

        void unique();

        template <class BinaryPredicate>
        void unique(BinaryPredicate pred);

        void sort();

        template <class Compare>
        void sort(Compare comp);
    };
    
    template <class T, class Alloc>
    template <class... Args>
    typename list<T, Alloc>::link_type list<T, Alloc>::create_node(Args&&... args) {
        link_type new_node = get_node();
        _alloc.construct(&(new_node -> data), HxSTL::forward<Args>(args)...);
        return new_node;
    }
    
    template <class T, class Alloc>
    inline void list<T, Alloc>::destroy_node(link_type p) {
        _alloc.destroy(&(p -> data));
        _alloc.deallocate(p, 1);
    }

    template<class T, class Alloc>
    void swap(list<T, Alloc>& x, list<T, Alloc>& y) {
        x.swap(y); 
    }

    template <class T, class Alloc>
    template <class InputIt>
    void list<T, Alloc>::initialize_aux(InputIt first, InputIt last, HxSTL::false_type) {
        _node = get_node();
        link_type cur_node = _node;
        link_type new_node;
        while (first != last) {
            new_node = create_node(*first);
            new_node -> prev = cur_node;
            cur_node -> next = new_node;
            cur_node = new_node;
            ++first;
        }
        _node -> prev = cur_node;
        cur_node -> next = _node;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::initialize_aux(size_type count, const T& value, HxSTL::true_type) {
        _node = get_node();
        link_type cur_node = _node;
        link_type new_node;
        while (count > 0) {
            new_node = create_node(value);
            new_node -> prev = cur_node;
            cur_node -> next = new_node;
            cur_node = new_node;
            --count;
        }
        _node -> prev = cur_node;
        cur_node -> next = _node;
    }
    
    template <class T, class Alloc>
    template <class InputIt>
    void list<T, Alloc>::assign_aux(InputIt first, InputIt last, HxSTL::false_type) {
        iterator first2 = begin();
        iterator last2 = end();
        while (first != last && first2 != last2) {
            *first2 = *first;
            ++first;
            ++first2;
        }
        if (first != last) {
            insert_aux(last2, first, last, HxSTL::false_type());
        } else if (first2 != last2) {
            erase_aux(first2, last2);
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::assign_aux(size_type count, const T& value, HxSTL::true_type) {
        iterator first = begin();
        iterator last = end();
        while (count > 0 && first != last) {
            *first = value;
            --count;
            ++first;
        }
        if (count > 0) {
            insert_aux(last, count, value, HxSTL::true_type());
        } else if (first != last) {
            erase_aux(first, last);
        }
    }

    template <class T, class Alloc>
    template <class... Args>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::emplace_aux(const_iterator pos, Args&&... args) {
        link_type new_node = create_node(HxSTL::forward<Args>(args)...);
        (pos._node -> prev) -> next =  new_node;
        new_node -> prev = pos._node -> prev;
        pos._node -> prev = new_node;
        new_node -> next = pos._node;
        return new_node;
    }

    template <class T, class Alloc>
    template <class U>
    typename list<T, Alloc>::const_iterator
    list<T, Alloc>::insert_aux(const_iterator pos, U&& value) {
        link_type new_node = create_node(HxSTL::forward<U>(value));
        (pos._node -> prev) -> next =  new_node;
        new_node -> prev = pos._node -> prev;
        pos._node -> prev = new_node;
        new_node -> next = pos._node;
        return new_node;
    }
    
    template <class T, class Alloc>
    template <class InputIt>
    typename list<T, Alloc>::const_iterator
    list<T, Alloc>::insert_aux(const_iterator pos, InputIt first, InputIt last, HxSTL::false_type) {
        link_type cur_node = pos._node -> prev;
        link_type pre_node = cur_node;
        link_type new_node;
        while (first != last) {
            new_node = create_node(*first);
            cur_node -> next = new_node;
            new_node -> prev = cur_node;
            cur_node = new_node;
            ++first;
        }
        pos._node -> prev = cur_node;
        cur_node -> next = pos._node;
        return pre_node -> next;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator
    list<T, Alloc>::insert_aux(const_iterator pos, size_type count, const T& value, HxSTL::true_type) {
        link_type cur_node = pos._node -> prev;
        link_type pre_node = cur_node;
        link_type new_node;
        while (count > 0) {
            new_node = create_node(value);
            cur_node -> next = new_node;
            new_node -> prev = cur_node;
            cur_node = new_node;
            --count;
        }
        pos._node -> prev = cur_node;
        cur_node -> next = pos._node;
        return pre_node -> next;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::erase_aux(const_iterator pos) {
        (pos._node -> next) -> prev = pos._node -> prev;
        (pos._node -> prev) -> next = pos._node -> next;
        const_iterator old_pos = pos;
        ++pos;
        destroy_node(old_pos._node);
        return pos;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::resize(size_type count, const T& value) {
        size_type sz = size();
        if (count > sz) {
            insert_aux(end(), count - sz, value, HxSTL::true_type());
        } else {
            iterator first = begin();
            HxSTL::advance(first, count);
            erase_aux(first, end());
        }
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::erase_aux(const_iterator first, const_iterator last) {
        (first._node -> prev) -> next = last._node;
        last._node -> prev = first._node -> prev;
        while (first != last) {
            const_iterator old = first;
            ++first;
            destroy_node(old._node);
        }
        return first;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::merge(list& other) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = other.begin();
        iterator last2 = other.end();

        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1) {
                iterator next = first2;
                splice(first1, other, first2, ++next);
                first2 = next;
            } else {
                ++first1;
            }
        }

        if (first2 != last2) {
            splice(first1, other, first2, last2);
        }
    }

    template <class T, class Alloc>
    template <class Compare>
    void list<T, Alloc>::merge(list& other, Compare comp) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = other.begin();
        iterator last2 = other.end();

        while (first1 != last1 && first2 != last2) {
            if (comp(*first2, *first1)) {
                iterator next = first2;
                splice(first1, other, first2, ++next);
                first2 = next;
            } else {
                ++first1;
            }
        }

        if (first2 != last2) {
            splice(first1, other, first2, last2);
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::splice(const_iterator pos, list&, const_iterator it) {
        it._node -> prev -> next = it._node -> next;
        it._node -> next -> prev = it._node -> prev;
        pos._node -> prev -> next = it._node;
        it._node -> prev = pos._node -> prev;
        pos._node -> prev = it._node;
        it._node -> next = pos._node;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::splice(const_iterator pos, list&, const_iterator first, const_iterator last) {
        link_type head = first._node -> prev;
        pos._node -> prev -> next = first._node;
        first._node -> prev = pos._node -> prev;
        pos._node -> prev = last._node -> prev;
        last._node -> prev -> next = pos._node;
        head -> next = last._node;
        last._node -> prev = head;
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::remove(const T& value) {
        const_iterator first = begin();
        const_iterator last = end();
        while (first != last) {
            if (*first == value) {
                first = erase_aux(first);
            } else {
                ++first;
            }
        }
    }
    
    template <class T, class Alloc>
    template <class UnaryPredicate>
    void list<T, Alloc>::remove_if(UnaryPredicate pred) {
        const_iterator first = begin();
        const_iterator last = end();
        while (first != last) {
            if (pred(*first)) {
                first = erase_aux(first);
            } else {
                ++first;
            }
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::reverse() {
        iterator first = begin();
        iterator last = end();
        _node -> prev = first;
        while (first != last) {
            iterator it = first._node -> next;
            first -> prev = it;
            it -> next = first;
            first = it;
        }
        _node -> next = first;
    }

    template <class T, class Alloc>
    void list<T, Alloc>::unique() {
        const_iterator first = begin();
        const_iterator last = end();
        if (first != last) {
            T value = *first;
            ++first;
            while (first != last) {
                if (*first == value) {
                    first = erase_aux(first);
                } else {
                    value = *first;
                    ++first;
                }
            }
        }
    }

    template <class T, class Alloc>
    template <class BinaryPredicate>
    void list<T, Alloc>::unique(BinaryPredicate pred) {
        const_iterator first = begin();
        const_iterator last = end();
        if (first != last) {
            T value = *first;
            ++first;
            while (first != last) {
                if (pred(*first, value)) {
                    first = erase_aux(first);
                } else {
                    value = *first;
                    ++first;
                }
            }
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::sort() {
        if (_node -> next == _node || _node -> next -> next == _node) return;

        list carry;
        list counter[64];
        list *max = &counter[0];
        list *cur = &counter[0];

        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            for (cur = &counter[0]; cur != max && !cur -> empty(); ++cur) {
                carry.merge(*cur);
            }
            carry.swap(*cur);
            if (cur == max) ++max;
        }

        for (cur = &counter[1]; cur != max; ++cur) {
            cur -> merge(cur[-1]);
        }
        swap(max[-1]);
    }

    template<class U, class A>
    bool operator==(const list<U, A>& lhs, const list<U, A>& rhs) {
        auto first1 = lhs.begin();
        auto last1 = lhs.end();
        auto first2 = rhs.begin();
        auto last2 = rhs.end(); 
        while (first1 != last1 && first2 != last2) {
            if (*first1 != *first2) {
                return false;
            }
            ++first1;
            ++first2;
        }
        return first1 == last1 && first2 == last2;
    }

    template<class U, class A>
    bool operator!=(const list<U, A>& lhs, const list<U, A>& rhs) {
        return !(lhs == rhs);
    }
    
    template<class U, class A>
    bool operator< (const list<U, A>& lhs, const list<U, A>& rhs) {
        // Todo
    }

    template<class U, class A>
    bool operator<=(const list<U, A>& lhs, const list<U, A>& rhs) {
        return !(lhs < rhs);
    }

    template<class U, class A>
    bool operator> (const list<U, A>& lhs, const list<U, A>& rhs) {
        return rhs < lhs;
    }

    template<class U, class A>
    bool operator>=(const list<U, A>& lhs, const list<U, A>& rhs) {
        return !(rhs < lhs);
    }

}


#endif
