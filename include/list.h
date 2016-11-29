#ifndef _LIST_H_
#define _LIST_H_


#include "allocator.h"
#include "uninitialized.h"


namespace HxSTL {

    template <class T>
    struct list_node {
        T data;
        list_node<T>* prev;
        list_node<T>* next;
    };

    template <class T>
    class list_iterator {
    public:
        typedef bidirectional_iterator_tag          iterator_category;
        typedef T                                   value_type;
        typedef T*                                  pointer;
        typedef T&                                  reference;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;

        typedef list_node<value_type>*              link_type;
    public:
        link_type _node;
    public:
        list_iterator() {}

        list_iterator(link_type node): _node(node) {}

        list_iterator(const list_iterator& other): _node(other._node) {}
        
        reference operator*() const { return _node -> data; }

        pointer operator->() const { return &(operator*()); }

        list_iterator& operator++() {
            _node = _node -> next;
            return *this;
        }

        list_iterator& operator++(int) {
            list_iterator& tmp = *this;
            ++*this;
            return tmp;
        }

        list_iterator& operator--() {
            _node = _node -> prev;
            return *this;
        }

        list_iterator& operator--(int) {
            list_iterator& tmp = *this;
            --*this;
            return tmp;
        }
    public:
        friend bool operator==(const list_iterator& lhs, const list_iterator& rhs) {
            return lhs._node == rhs._node;
        }

        friend bool operator!=(const list_iterator& lhs, const list_iterator& rhs) {
            return lhs._node != rhs._node;
        }
    };

    template <class T, class Alloc = allocator<list_node<T>>>
    class list {
    public:
        typedef T                                           value_type;
        typedef Alloc                                       allocator_type;
        typedef value_type&                                 reference;
        typedef const value_type&                           const_reference;
        typedef value_type*                                 pointer;
        typedef const value_type*                           const_pointer;
        typedef list_iterator<value_type>                   iterator;
        typedef list_iterator<const value_type>             const_iterator;
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
        void initialize_aux(InputIt first, InputIt last, false_type);
        void initialize_aux(size_type count, const T& value, true_type);
        template <class U>
        iterator insert_aux(const_iterator pos, U&& value);
        template <class InputIt>
        iterator insert_aux(const_iterator pos, InputIt first, InputIt last, false_type);
        iterator insert_aux(const_iterator pos, size_type count, const T& value, true_type);
        template <class... Args>
        iterator emplace_aux(const_iterator pos, Args&&... args);
        template <class InputIt>
        void assign_aux(InputIt first, InputIt last, false_type);
        void assign_aux(size_type n, const T& value, true_type);
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
            initialize_aux(first, last, typename HxSTL::is_integeral<InputIt>::value());
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

        ~list() {
            erase(begin(), end());
            destroy_node(_node);
        }

        list& operator=(const list& other) {
            list(other).swap(*this);
            return *this;
        }

        list& operator=(list&& other) {
            list(HxSTL::move(other)).swap(*this);
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
        
        void clear() { erase(begin(), end()); }

        iterator insert(const_iterator pos, const T& value) {
            return insert_aux(pos, value);
        }

        iterator insert(const_iterator pos, T&& value) {
            return insert_aux(pos, HxSTL::move(value));
        }

        iterator insert(iterator pos, size_type count, const T& value) {
            return insert_aux(pos, count, value, HxSTL::true_type());
        }

        template <class InputIt>
        iterator insert(iterator pos, InputIt first, InputIt last) {
            return insert_aux(pos, first, last, typename HxSTL::is_integeral<InputIt>::type());
        }

        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args) {
            return emplcae_aux(pos, HxSTL::forward<Args>(args)...);
        }

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);

        void push_back(const T& value) { insert_aux(end(), value); }

        void push_back(T&& value) { insert_aux(end(), HxSTL::move(value)); }

        template <class... Args>
        void emplace_back(Args&&... args) { emplace_aux(end(), HxSTL::forward<Args>(args)...); }

        void pop_back() { erase(--end()); }

        void push_front(const T& value) { insert_aux(begin(), value); }

        void push_front(T&& value) { insert_aux(begin(), HxSTL::move(value)); }

        template <class... Args>
        void emplace_front(Args&&... args) { emplace_aux(begin(), HxSTL::forward<Args>(args)...); }

        void pop_front() { erase(begin()); }

        void resize(size_type count);

        void resize(size_type count, const T& value);

        void swap(list& other) {
            HxSTL::swap(_node, other._node);
        }

        void merge(list& other);

        void merge(list&& other);

        template <class Compare>
        void merge(list& other, Compare comp);

        template <class Compare>
        void merge(list&& other, Compare comp);

        void splice(const_iterator pos, list& other);

        void splice(const_iterator pos, list&& other);

        void splice(const_iterator pos, list& other, const_iterator it);

        void splice(const_iterator pos, list&& other, const_iterator it);

        void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

        void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last);

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

    template<class T, class Alloc>
    void swap(list<T, Alloc>& x, list<T, Alloc>& y) {
        x.swap(y); 
    }

    template <class T, class Alloc>
    template <class InputIt>
    void list<T, Alloc>::initialize_aux(InputIt first, InputIt last, false_type) {
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
    void list<T, Alloc>::initialize_aux(size_type count, const T& value, true_type) {
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
    void list<T, Alloc>::assign_aux(InputIt first, InputIt last, false_type) {
        iterator start = begin();
        iterator finish = end();
        while (first != last && start != finish) {
            *start = *first;
            ++first;
            ++start;
        }
        if (first != last) {
            insert(finish, first, last);
        } else if (start != finish) {
            erase(start, finish);
        }
    }

    template <class T, class Alloc>
    void list<T, Alloc>::assign_aux(size_type count, const T& value, true_type) {
        iterator start = begin();
        iterator finish = end();
        while (count > 0 && start != finish) {
            *start = value;
            --count;
            ++start;
        }
        if (count > 0) {
            push_back(count, value);
        } else if (start != finish) {
            erase(start, finish);
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
        return reinterpret_cast<iterator>(pos);
    }

    template <class T, class Alloc>
    template <class U>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert_aux(const_iterator pos, U&& value) {
        link_type new_node = create_node(HxSTL::forward<U>(value));
        (pos._node -> prev) -> next =  new_node;
        new_node -> prev = pos._node -> prev;
        pos._node -> prev = new_node;
        new_node -> next = pos._node;
        return reinterpret_cast<iterator>(pos);
    }
    
    template <class T, class Alloc>
    template <class InputIt>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert_aux(const_iterator pos, InputIt first, InputIt last, false_type) {
        link_type cur_node = pos._node -> prev;
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
        return reinterpret_cast<iterator>(pos);
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert_aux(const_iterator pos, size_type count, const T& value, true_type) {
        link_type cur_node = pos._node -> prev;
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
        return reinterpret_cast<iterator>(pos);
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position) {
        if (position != end()) {
            (position._node -> next) -> prev = position._node -> prev;
            (position._node -> prev) -> next = position._node -> next;
            iterator tmp = position;
            ++position;
            destroy_node(tmp._node);
        }
        return position;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last) {
        if (first != end()) {
            (first._node -> prev) -> next = last._node;
            last._node -> prev = first._node -> prev;
            while (first != last) {
                iterator tmp = first;
                ++first;
                destroy_node(tmp._node);
            }
        }
        return first;
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::resize(size_type n, value_type val) {
        size_type sz = size();
        if (n > sz) {
            insert(end(), n - sz, val);
        } else {
            iterator first = begin();
            advance(first, n);
            erase(first, end());
        }
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list& x) {
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list& x, iterator i) {
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list& x, iterator first, iterator last) {
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::remove(const value_type& val) {
    }
    
    template <class T, class Alloc>
    template <class Predicate>
    void list<T, Alloc>::remove_if(Predicate pred) {
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::unique() {
    }
    
    template <class T, class Alloc>
    template <class BinaryPredicate>
    void list<T, Alloc>::unique(BinaryPredicate binary_pred) {
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::merge(list& x) {
    }
    
    template <class T, class Alloc>
    template <class Compare>
    void list<T, Alloc>::merge(list& x, Compare comp) {
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::sort() {
    }
    
    template <class T, class Alloc>
    template <class Compare>
    void list<T, Alloc>::sort(Compare comp) {
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::reverse() {
    }
    
    template <class T, class Alloc>
    inline void list<T, Alloc>::destroy_node(link_type p) {
        destroy(&(p -> data));
        _alloc.deallocate(p, 1);
    }

    template<class U, class A>
    bool operator==(const list<U, A>& lhs, const list<U, A>& rhs) {
        // Todo
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
