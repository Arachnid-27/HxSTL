#ifndef _DEQUE_H_
#define _DEQUE_H_


#include "allocator.h"
#include "uninitialized.h"


namespace HxSTL {

    template <class T>
    class deque_iterator {
    protected:
        enum {__BUFFER_SIZE = 512};
    public:
        typedef bidirectional_iterator_tag          iterator_category;
        typedef T                                   value_type;
        typedef value_type*                         pointer;
        typedef value_type&                         reference;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;

        typedef pointer*                            map_pointer;
    public:
        pointer _cur;
        pointer _first;
        pointer _last;
        map_pointer _node;
    public:
        size_t buffer_size() { return __BUFFER_SIZE / sizeof(T); }

        void set_node(map_pointer new_node) {
            _node = new_node;
            _first = *new_node;
            _last = _first + buffer_size();
        }
    public:
        reference operator*() const { return *_cur; }
        pointer operator->() const { return &(operator*()); }

        difference_type operator-(const deque_iterator& x) const {
            return buffer_size() * (_node - x._node - 1) + (_cur - _first) + (x._last - x._cur);
        }

        deque_iterator& operator++() {
            ++_cur;
            if (_cur == _last) {
                set_node(_node + 1);
                _cur = _first;
            }
            return *this;
        }

        deque_iterator operator++(int) {
            deque_iterator tmp = *this;
            ++*this;
            return tmp;
        }

        deque_iterator& operator--() {
            if (_cur == _first) {
                set_node(_node - 1);
                _cur = _last;
            }
            --_cur;
            return *this;
        }

        deque_iterator operator--(int) {
            deque_iterator tmp = *this;
            --*this;
            return tmp;
        }

        deque_iterator& operator+=(difference_type n) {
            difference_type offset = n + (_cur - _first);
            if (offset >= 0 && offset < buffer_size()) {
                _cur += n;
            } else {
                difference_type node_offset = 
                    offset > 0 ? offset / buffer_size() : -((-offset - 1) / buffer_size()) - 1;
                set_node(_node + node_offset);
                _cur = _first + (offset - node_offset * buffer_size());
            }
            return *this;
        }

        deque_iterator operator+(difference_type n) const {
            deque_iterator tmp = *this;
            return tmp += n;
        }

        deque_iterator& operator-=(difference_type n) {
            return *this += -n;
        }

        deque_iterator operator-(difference_type n) const {
            deque_iterator tmp = *this;
            return tmp -= n;
        }

        reference operator[](difference_type n) const {
            return *(*this + n);
        }

        bool operator==(const deque_iterator& x) const { return _cur == x._cur; }
        bool operator!=(const deque_iterator& x) const { return _cur != x._cur; }
        bool operator< (const deque_iterator& x) const {
            return (_node == x._node) ? (_cur < x._cur) : (_node < x._node);
        }
    };
    

    template <class T, class Alloc = allocator<T> >
    class deque {
    public:
        typedef T                                           value_type;
        typedef Alloc                                       allocator_type;
        typedef value_type&                                 reference;
        typedef const value_type&                           const_reference;
        typedef value_type*                                 pointer;
        typedef const value_type*                           const_pointer;
        typedef deque_iterator<value_type>                  iterator;
        typedef deque_iterator<const value_type>            const_iterator;
    //  typedef reverse_iterator<iterator>                  reverse_iterator;
    //  typedef reverse_iterator<const_iterator>            const_reverse_iterator;
        typedef ptrdiff_t                                   difference_type;
        typedef size_t                                      size_type;

        typedef typename iterator::map_pointer              map_pointer;
        typedef allocator<pointer>                          map_allocator_type;
    protected:
        allocator_type _alloc;
        map_allocator_type _map_alloc; 
        iterator _start;
        iterator _finish;
        map_pointer _map;
        size_type _map_size;
    protected:
        template <class InputIterator>
        void initialize_aux(InputIterator first, InputIterator last, false_type);
        void initialize_aux(size_type n, const value_type& val, true_type);
        void create_map(size_type num_elements);
    public:
        explicit deque(const allocator_type& alloc = allocator_type())
            :_alloc(alloc), _map_alloc(map_allocator_type()) 
            { create_map(0); }

        explicit deque(size_type n, const value_type& val = value_type(), 
                const allocator_type& alloc = allocator_type())
            :_alloc(alloc), _map_alloc(map_allocator_type()) 
            { initialize_aux(n, val, true_type()); }

        template <class InputIterator>
        deque(InputIterator first, InputIterator last, 
                const allocator_type& alloc = allocator_type())
            :_alloc(alloc), _map_alloc(map_allocator_type()) 
            { initialize_aux(first, last, typename is_integer<InputIterator>::value()); }

        deque(const deque& x);

        ~deque();

        deque& operator=(const deque& x);
        reference operator[](size_type n) { return *(_start + n); }
        const_reference operator[](size_type n) const { return *(_start + n); }
        
        iterator begin() { return _start; }
        const_iterator begin() const { return _start; }
        iterator end() { return _finish; }
        const_iterator end() const { return _finish; }
    //  reverse_iterator rbegin();
    //  const_reverse_iterator rbegin() const;
    //  reverse_iterator rend();
    //  const_reverse_iterator rend() const;
    
        size_type size() const { return _finish - _start; }
        size_type max_size() const { return _alloc.max_size(); }
        void resize(size_type n, value_type val = value_type());
        bool empty() const { return _start == _finish; }

    
        reference at(size_type n) { return *(_start + n); }
        const_reference at(size_type n) const { return *(_start + n); }
        reference front() { return *_start; }
        const_reference front() const { return *_start; }
        reference back() { return *(_finish - 1); }
        const_reference back() const { return *(_finish - 1); }
        
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last);
        void assign(size_type n, const value_type& val); 
        void push_back(const value_type& val);
        void pop_back();
        iterator insert(iterator position, const value_type& val);
        void insert(iterator position, size_type n, const value_type& val);
        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void swap(deque& x);
        void clear();

        allocator_type get_allocator() const { return _alloc; }
    public:
        template<class U, class A>
        friend bool operator==(const deque<U, A>& lhs, const deque<U, A>& rhs);
        template<class U, class A>
        friend bool operator!=(const deque<U, A>& lhs, const deque<U, A>& rhs);
        template<class U, class A>
        friend bool operator< (const deque<U, A>& lhs, const deque<U, A>& rhs);
        template<class U, class A>
        friend bool operator<=(const deque<U, A>& lhs, const deque<U, A>& rhs);
        template<class U, class A>
        friend bool operator> (const deque<U, A>& lhs, const deque<U, A>& rhs);
        template<class U, class A>
        friend bool operator>=(const deque<U, A>& lhs, const deque<U, A>& rhs);
        template<class U, class A>
        friend void swap(deque<U, A>& x, deque<U, A>& y);
    };

    template <class T, class Alloc>
    template <class InputIterator>
    void deque<T, Alloc>::initialize_aux(InputIterator first, InputIterator last, false_type) {
        create_map(distance(first, last));

        iterator p = _start;
        try {
            while (first != last) {
                construct(p._cur, *first);
                ++p;
                ++first;
            }
        } catch (...) {
            while (p != first) {
                destroy(p, sizeof(T));
                --p;
            }
            // Todo
            throw;
        }
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::initialize_aux(size_type n, const value_type& val, true_type) {
        create_map(n);

        map_pointer cur = _start._node;
        try {
            while (cur != _finish._node) {
                uninitialized_fill_n(*cur, n, val);
                ++cur;
            }
            uninitialized_fill(_finish._first, _finish._cur, val);
        } catch (...) {
            // Todo
            throw;
        }
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::create_map(size_type num_elements) {
        size_type num_nodes = num_elements / _start.buffer_size() + 1;

        _map_size = num_nodes > 6 ? num_nodes + 2 : 8;
        _map = _map_alloc.allocate(_map_size);   

        map_pointer start = _map + (_map_size - num_nodes) / 2;
        map_pointer finish = _start + num_nodes - 1;

        map_pointer cur = start;
        try {
            while (cur != finish) {
                cur = _alloc.allocate(_start.buffer_size());
                ++cur;
            }
        } catch (...) {
            while (start != cur) {
                _alloc.deallocate(start, _start.buffer_size());
                ++start;
            }
            throw;
        }

        _start.set_node(start);
        _finish.set_node(finish);
        _start.cur = _start.first;
        _finish.cur = _finish.first + num_elements % _start.buffer_size();
    }

}


#endif
