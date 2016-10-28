#ifndef _VECTOR_H_
#define _VECTOR_H_


#include "allocator.h"
#include "uninitialized.h"


namespace HxSTL {

    template <class T, class Alloc = allocator<T> >
    class vector {
    public:
        typedef T                                       value_type;
        typedef Alloc                                   allocator_type;
        typedef value_type&                             reference;
        typedef const value_type&                       const_reference;
        typedef value_type*                             pointer;
        typedef const value_type*                       const_pointer;
        typedef value_type*                             iterator;
        typedef const value_type*                       const_iterator;
    //  typedef reverse_iterator<iterator>              reverse_iterator;
    //  typedef reverse_iterator<const_iterator>        const_reverse_iterator;
        typedef ptrdiff_t                               difference_type;
        typedef size_t                                  size_type;
    protected:
        iterator _start;
        iterator _finish;
        iterator _end;
        allocator_type _alloc;
    protected:
        template <class InputIterator>
        void initialize_aux(InputIterator first, InputIterator last, false_type);
        void initialize_aux(size_type n, const value_type& val, true_type);
        template <class InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, false_type);
        void insert_aux(iterator position, size_type n, const T& x, true_type);
        void allocate_and_single_insert(iterator position, const T& x);
        void destroy_and_initialize(iterator new_start, iterator new_finish, iterator new_end);
    public:
        explicit vector(const allocator_type& alloc = allocator_type())
            : _start(0), _finish(0), _end(0), _alloc(alloc) {}
        explicit vector(size_type n, const value_type& val = value_type(), 
                const allocator_type& alloc = allocator_type());
        template <class InputIterator>
        vector(InputIterator first, InputIterator last, 
                const allocator_type& alloc = allocator_type());
        vector(const vector& x);

        ~vector();
        
        vector& operator=(const vector& x) { assign(x.begin(), x.end()); }
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
        size_type capacity() const { return _end - _start; }
        bool empty() const { return _start == _finish; }
        void reserve(size_type n); 

        reference at(size_type n) { return *(_start + n); }
        const_reference at(size_type n) const { return *(_start + n); }
        reference front() { return *_start; }
        const_reference front() const { return *_start; }
        reference back() { return *(_finish - 1);}
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
        void swap(vector& x);
        void clear() { erase(_start, _finish); }

        allocator_type get_allocator() const { return _alloc; }
    public:
        friend bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
        friend bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
        friend bool operator< (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
        friend bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
        friend bool operator> (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
        friend bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
        friend void swap(vector<T, Alloc>& x, vector<T, Alloc>& y);
    };

    template <class T, class Alloc>
    vector<T, Alloc>::vector(size_type n, const value_type& val, 
            const allocator_type& alloc): _alloc(alloc) {
        initialize_aux(n, val, true_type());
    }

    template <class T, class Alloc>
    template <class InputIterator>
    vector<T, Alloc>::vector(InputIterator first, InputIterator last, 
            const allocator_type& alloc): _alloc(alloc) {
        initialize_aux(first, last, typename is_integer<InputIterator>::value());
    }

    template<class T, class Alloc>
    vector<T, Alloc>::vector(const vector<T, Alloc>& x): _alloc(x._alloc) {
        _start = _alloc.allocate(x.capacity());
        _finish = uninitialized_copy(x.begin(), x.end(), _start);
        _end = _start + x.capacity();
    }

    template <class T, class Alloc>
    template <class InputIterator>
    inline void vector<T, Alloc>::initialize_aux(InputIterator first, InputIterator last, false_type) {
        _start = _alloc.allocate(last - first);
        _finish = uninitialized_copy(first, last, _start);
        _end = _finish;
    }

    template <class T, class Alloc>
    inline void vector<T, Alloc>::initialize_aux(size_type n, const value_type& val, true_type) {
        _start = _alloc.allocate(n);
        _finish = uninitialized_fill_n(_start, n, val);
        _end = _start + n;
    }

    template <class T, class Alloc>
    vector<T, Alloc>::~vector() {
        if (_start) {
            destroy(_start, _finish);
            _alloc.deallocate(_start, capacity());
        }
    }

    template <class T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& x) {
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::resize(size_type n, value_type val) {
        if (n < size()) {
            destroy(_start + n, _finish);
            _finish = _start + n;
        } else if (n < capacity()) {
            _finish = uninitialized_fill_n(_finish, n - size(), val);
        } else {
            iterator new_start = _alloc.allocate(n);
            iterator new_finish = new_start;

            try {
                new_finish = uninitialized_copy(_start, _finish, new_start);
                new_finish = uninitialized_fill_n(new_finish, n - size(), val);
            } catch (...) {
                destroy(new_start, new_finish);
                _alloc.deallocate(new_start, n);
                throw;
            }

            destroy_and_initialize(new_start, new_finish, new_finish);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::reserve(size_type n) {
        if (n > capacity()) {
            iterator new_start = _alloc.allocate(n);
            iterator new_finish = new_start;

            try {
                new_finish = uninitialized_copy(_start, _finish, new_start);
            } catch (...) {
                destroy(new_start, new_finish);
                _alloc.deallocate(new_start, n);
                throw;
            }

            destroy_and_initialize(new_start, new_finish, new_start + n);
        }
    }

    template <class T, class Alloc>
    template <class InputIterator>
    void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
        size_type n = last - first;

        if (n <= capacity()) {
            iterator old_finish = _finish;

            copy(first, last, _start);
            _finish = _start + n;

            destroy(_finish, old_finish);
        } else {
            iterator new_start = _alloc.allocate(n);
            iterator new_finish = new_start;

            try {
                new_finish = uninitialized_copy(first, last, new_start);
            } catch (...) {
                destroy(new_start, new_finish);
                _alloc.deallocate(new_start, n);
                throw;
            }

            destroy_and_initialize(new_start, new_finish, new_finish);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::assign(size_type n, const value_type& val) {
        if (n <= capacity()) {
            iterator old_finish = _finish;

            fill_n(_start, n, val);
            _finish = _start + n; 

            destroy(_finish, old_finish);
        } else {
            iterator new_start = _alloc.allocate(n);
            iterator new_finish = new_start;

            try {
                new_finish = uninitialized_fill_n(n, val, new_start);
            } catch (...) {
                destroy(new_start, new_finish);
                _alloc.deallocate(new_start, n);
                throw;
            }

            destroy_and_initialize(new_start, new_finish, new_finish);
        }
    } 

    template <class T, class Alloc>
    void vector<T, Alloc>::push_back(const value_type& val) {
        if (_finish != _end) {
            construct(_finish, val);
            ++_finish;
        } else {
            allocate_and_single_insert(_finish, val);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::pop_back() {
        --_finish;
        destroy(_finish);
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& val) {
        if (_finish != _end) {
            ++_finish;
            copy_backward(position, _finish - 1, _finish);
            *position = val;
        } else {
            allocate_and_single_insert(position, val);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::insert(iterator position, size_type n, const value_type& val) {
        if (n > 0) {
            insert_aux(position, n, val, true_type());
        }
    }

    template <class T, class Alloc>
    template <class InputIterator>
    void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
        insert_aux(position, first, last, typename is_integer<InputIterator>::value());
    }

    template <class T, class Alloc>
    template <class InputIterator>
    inline void vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, false_type) {
        size_type n = last - first;

        if (_end - _finish >= n) {
            size_type element_after = _finish - position;

            iterator old_finish = _finish;
            if (n > element_after) {
                _finish = uninitialized_copy(first + element_after, last, _finish);
                _finish = uninitialized_copy(position, old_finish, _finish);
                copy_n(first, element_after, position);
            } else {
                _finish = uninitialized_copy(_finish - n, _finish, _finish);
                copy_backward(position, old_finish - n, old_finish);
                copy(first, last, position);
            }
        } else {
            size_type new_sz = 2 * capacity() > n ? 2 * capacity() : n;

            iterator new_start = _alloc.allocate(new_sz);
            iterator new_finish = new_start;

            try {
                new_finish = uninitialized_copy(_start, position, new_start);
                new_finish = uninitialized_copy(first, last, new_finish);
                new_finish = uninitialized_copy(position, _finish, new_finish);
            } catch (...) {
                destroy(new_start, new_finish);
                _alloc.deallocate(new_start, new_sz);
                throw;
            }

            destroy_and_initialize(new_start, new_finish, new_start + new_sz);
        }
    }

    template <class T, class Alloc>
    inline void vector<T, Alloc>::insert_aux(iterator position, size_type n, const value_type& val, true_type) {
        if (_end - _finish >= n) {
            size_type element_after = _finish - position;

            iterator old_finish = _finish;
            if (n > element_after) {
                _finish = uninitialized_fill_n(_finish, n - element_after, val);
                _finish = uninitialized_copy(position, old_finish, _finish);
                fill_n(position, element_after, val);
            } else {
                _finish = uninitialized_copy(_finish - n, _finish, _finish);
                copy_backward(position, old_finish - n, old_finish);
                fill_n(position, n, val);
            }
        } else {
            size_type new_sz = 2 * capacity() > n ? 2 * capacity(): n;

            iterator new_start = _alloc.allocate(new_sz);
            iterator new_finish = new_start;

            try {
                new_finish = uninitialized_copy(_start, position, new_start);
                new_finish = uninitialized_fill_n(new_finish, n, val);
                new_finish = uninitialized_copy(position, _finish, new_finish);
            } catch (...) {
                destroy(new_start, new_finish);
                _alloc.deallocate(new_start, new_sz);
                throw;
            }

            destroy_and_initialize(new_start, new_finish, new_start + new_sz);
        }
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
        _finish = copy(position + 1, _finish, position);

        destroy(_finish);
        return position;
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
        iterator old_finish = _finish;
        _finish = copy(last, _finish, first);

        destroy(_finish, old_finish);
        return first;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::swap(vector& x) {
        iterator temp_iterator;
        
        temp_iterator = _start;
        _start = x._start;
        x._start = temp_iterator;

        temp_iterator = _finish;
        _finish = x._finish;
        x._finish = temp_iterator;

        temp_iterator = _end;
        _end = x._end;
        x._end = temp_iterator;
    }

    template <class T, class Alloc>
    inline void vector<T, Alloc>::allocate_and_single_insert(iterator position, const T& x) {
        size_type new_sz = size() ? 2 * size() : 1;

        iterator new_start = _alloc.allocate(new_sz);
        iterator new_finish = new_start;

        try {
            new_finish = uninitialized_copy(_start, position, new_start);
            construct(new_finish, x);
            ++new_finish;
            new_finish = uninitialized_copy(position, _finish, new_finish);
        } catch (...) {
            destroy(new_start, new_finish);
            _alloc.deallocate(new_start, new_sz);
            throw;
        }

        destroy_and_initialize(new_start, new_finish, new_start + new_sz);
    }

    template <class T, class Alloc>
    inline void vector<T, Alloc>::destroy_and_initialize(iterator new_start, 
            iterator new_finish, iterator new_end) {
        if (_start) {
            destroy(_start, _finish);
            _alloc.deallocate(_start, capacity());
        }

        _start = new_start;
        _finish = new_finish;
        _end = new_end;
    }

    /*
     * friend
     */

    template <class T, class Alloc>
    bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
        // Todo
    }

    template <class T, class Alloc>
    bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
        return !(lhs == rhs);
    }
    
    template <class T, class Alloc>
    bool operator< (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
        // Todo
    }

    template <class T, class Alloc>
    bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
        return !(lhs < rhs);
    }

    template <class T, class Alloc>
    bool operator> (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
        return rhs < lhs;
    }

    template <class T, class Alloc>
    bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
        return !(rhs < lhs);
    }

    template <class T, class Alloc>
    inline void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
        x.swap(y);
    }

}


#endif
