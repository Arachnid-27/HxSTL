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

    template <class T, class Ref, class Ptr>
    class list_iterator {
    public:
        typedef bidirectional_iterator_tag          iterator_category;
        typedef T                                   value_type;
        typedef Ptr                                 pointer;
        typedef Ref                                 reference;
        typedef list_node<value_type>*              link_type;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;
    protected:
        link_type _node;
    public:
        list_iterator() {}
        list_iterator(link_type x): _node(x) {}
        list_iterator(const list_iterator<T, Ref, Ptr>& x): _node(x._node) {}
        
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

    template <class T, class Alloc = allocator<list_node<T> > >
    class list {
    public:
        typedef T                                                       value_type;
        typedef Alloc                                                   allocator_type;
        typedef value_type&                                             reference;
        typedef const value_type&                                       const_reference;
        typedef value_type*                                             pointer;
        typedef const value_type*                                       const_pointer;
        typedef list_iterator<value_type, reference, pointer>           iterator;
        typedef const list_iterator<value_type, reference, pointer>     const_iterator;
    //  typedef reverse_iterator<iterator>                              reverse_iterator;
    //  typedef reverse_iterator<const_iterator>                        const_reverse_iterator;
        typedef ptrdiff_t                                               difference_type;
        typedef size_t                                                  size_type;

        typedef typename iterator::link_type                            link_type;
    protected:
        link_type _node;
        allocator_type _alloc;
    protected:
        template <class InputIterator>
        void initialize_aux(InputIterator first, InputIterator last, false_type);
        void initialize_aux(size_type n, const value_type& val, true_type);
        link_type get_node() { return _alloc.allocate(1); }
        link_type create_node(const T& x);
        void destroy_node(link_type p);
    public:
        explicit list(const allocator_type& alloc = allocator_type());
        explicit list(size_type n, const value_type& val = value_type(), 
                const allocator_type& alloc = allocator_type());
        template <class InputIterator>
        list(InputIterator first, InputIterator last, 
                const allocator_type& alloc = allocator_type());
        list(const list& x); 

        ~list();

        list& operator=(const list& x);

        iterator begin() { return _node -> next; }
        const_iterator begin() const { return _node -> next; }
        iterator end() { return _node; }
        const_iterator end() const { return _node; }
    //  reverse_iterator rbegin();
    //  const_reverse_iterator rbegin() const;
    //  reverse_iterator rend();
    //  const_reverse_iterator rend() const;
    
        bool empty() const { return _node == _node -> next; }
        size_type size() const { return distance(begin(), end()); }
        size_type max_size() const { return _alloc.max_size(); }

        reference front() { return *begin(); }
        const_reference front() const { return *begin(); }
        reference back() { return *end(); }
        const_reference back() const { return *end(); }
        
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last);
        void assign(size_type n, const value_type& val); 
        void push_front(const value_type& val) { insert(begin(), val); }
        void pop_front() { erase(begin()); }
        void push_back(const value_type& val) { insert(end(), val); }
        void pop_back() { erase(end()); }
        iterator insert(iterator position, const value_type& val);
        void insert(iterator position, size_type n, const value_type& val);
        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void swap(list& x);
        void resize(size_type n, value_type val = value_type());
        void clear() { erase(begin(), end()); }

        void splice(iterator position, list& x);
        void splice(iterator position, list& x, iterator i);
        void splice(iterator position, list& x, iterator first, iterator last);
        void remove(const value_type& val);
        template <class Predicate>
        void remove_if(Predicate pred);
        void unique();
        template <class BinaryPredicate>
        void unique(BinaryPredicate binary_pred);
        void merge(list& x);
        template <class Compare>
        void merge(list& x, Compare comp);
        void sort();
        template <class Compare>
        void sort(Compare comp);
        void reverse();

        allocator_type get_allocator() const { return _alloc; }
    public:
        friend bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs);
        friend bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs);
        friend bool operator< (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs);
        friend bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs);
        friend bool operator> (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs);
        friend bool operator>=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs);
        friend void swap(list<T, Alloc>& x, list<T, Alloc>& y);
    };

    template <class T, class Alloc>
    list<T, Alloc>::list(const allocator_type& alloc): _alloc(alloc) {
        _node = get_node();
        _node -> next = _node;
        _node -> prev = _node;
    }

    template <class T, class Alloc>
    list<T, Alloc>::list(size_type n, const value_type& val, 
            const allocator_type& alloc): _alloc(alloc) {
        initialize_aux(n, val, true_type());
    }

    template <class T, class Alloc>
    template <class InputIterator>
    list<T, Alloc>::list(InputIterator first, InputIterator last, 
            const allocator_type& alloc): _alloc(alloc) {
        initialize_aux(first, last, typename is_integer<InputIterator>::value());
    }

    template <class T, class Alloc>
    list<T, Alloc>::list(const list& x): _alloc(x._alloc) {
        initialize_aux(x.begin(), x.end(), false_type());
    }

    template <class T, class Alloc>
    list<T, Alloc>::~list() {
    }

    template <class T, class Alloc>
    template <class InputIterator>
    inline void list<T, Alloc>::initialize_aux(InputIterator first, InputIterator last, false_type) {
        _node = get_node();
        link_type p = _node;
        while (first != last) {
            link_type tmp = create_node(*first);
            tmp -> prev = p;
            p -> next = tmp;
            p = tmp;
            ++first;
        }
        _node -> prev = p;
        p -> next = _node;
    }

    template <class T, class Alloc>
    inline void list<T, Alloc>::initialize_aux(size_type n, const value_type& val, true_type) {
        _node = get_node();
        link_type p = _node;
        while (n != 0) {
            link_type tmp = create_node(val);
            tmp -> prev = p;
            p -> next = tmp;
            p = tmp;
            --n;
        }
        _node -> prev = p;
        p -> next = _node;
    }

    template <class T, class Alloc>
    list<T, Alloc>& list<T, Alloc>::operator=(const list& x) {
    }
    
    template <class T, class Alloc>
    template <class InputIterator>
    void list<T, Alloc>::assign(InputIterator first, InputIterator last) {
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::assign(size_type n, const value_type& val) {
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::insert(iterator position, const value_type& val) {
        link_type tmp = create_node(val);
        tmp -> next = position._node;
        tmp -> prev = position._node -> prev;
        (position._node -> prev) -> next = tmp;
        position._node -> prev = tmp;
        return tmp;
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::insert(iterator position, size_type n, const value_type& val) {
    }
    
    template <class T, class Alloc>
    template <class InputIterator>
    void list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position) {
        if (position._node != _node) {
            (position -> next) -> prev = position -> prev;
            (position -> prev) -> next = position -> next;
            destroy_node(position._node);
        }
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last) {
        if (first._node != _node) {
            (first._node -> prev) -> next = last._node;
            last._node -> prev = first._node -> prev;
            while (first != last) {
                destroy_node(first._node);
                ++first;
            }
        }
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::swap(list& x) {
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::resize(size_type n, value_type val) {
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
    inline typename list<T, Alloc>::link_type list<T, Alloc>::create_node(const T& x) {
        link_type p = get_node();
        construct(&(p -> data), x);
        return p;
    }
    
    template <class T, class Alloc>
    inline void list<T, Alloc>::destroy_node(link_type p) {
        destroy(&(p -> data));
        _alloc.deallocate(p, 1);
    }

}


#endif
