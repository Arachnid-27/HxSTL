#ifndef _VECTOR_H_
#define _VECTOR_H_


#include "allocator.h"
#include "uninitialized.h"


namespace HxSTL {

    template <class T, class Alloc = allocator<T>>
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
        iterator _end_of_storage;
        allocator_type _alloc;
    protected:
        template <class InputIterator>
        void initialize_aux(InputIterator first, InputIterator last, false_type);
        void initialize_aux(size_type count, const value_type& value, true_type);
        void initialize_aux(const vector& other);
        void initialize_aux(vector&& other);
        template <class InputIterator>
        iterator insert_aux(iterator pos, InputIterator first, InputIterator last, false_type);
        iterator insert_aux(iterator pos, size_type count, const T& value, true_type);
        iterator insert_aux(iterator pos, T&& value);
        template <class InputIterator>
        void assign_aux(InputIterator first, InputIterator last, false_type);
        void assign_aux(size_type count, const T& value, true_type);
        iterator allocate_and_single_insert(iterator pos, T&& value);
        void destroy_and_initialize(iterator new_start, iterator new_finish, iterator new_end_of_storage);
    public:
        explicit vector(const allocator_type& alloc = allocator_type())
            : _start(0), _finish(0), _end_of_storage(0), _alloc(alloc) {}

        explicit vector(size_type count, const value_type& value = value_type(), 
                const allocator_type& alloc = allocator_type())
            : _alloc(alloc) { initialize_aux(count, value, true_type()); }

        template <class InputIterator>
        vector(InputIterator first, InputIterator last, 
                const allocator_type& alloc = allocator_type())
            : _alloc(alloc) { initialize_aux(first, last, typename is_integer<InputIterator>::value()); }

        vector(const vector& other): _alloc(other._alloc) { initialize_aux(other); }

        vector(vector&& other): _alloc(HxSTL::move(other._alloc)) { initialize_aux(HxSTL::move(other)); }

        ~vector() {
            if (_start) {
                destroy(_start, _finish);
                _alloc.deallocate(_start, capacity());
            }
        }
        
        vector& operator=(const vector& other) {
            assign(other.begin(), other.end());
            return *this;
        }

        vector& operator=(vector&& other) {
        }

        template <class InputIterator>
        void assign(InputIterator first, InputIterator last) {
            assign_aux(first, last, typename is_integer<InputIterator>::value());
        }

        void assign(size_type n, const value_type& value) {
            assign_aux(n, value, true_type());
        }

        allocator_type get_allocator() const { return _alloc; }

        reference at(size_type n) { /*需要检查边界*/ return *(_start + n); }

        const_reference at(size_type n) const { return *(_start + n); }

        reference operator[](size_type n) { return *(_start + n); }

        const_reference operator[](size_type n) const { return *(_start + n); }

        reference front() { return *begin(); }

        const_reference front() const { return *begin(); }

        reference back() { return *(end() - 1);}

        const_reference back() const { return *(end() - 1); }

        pointer data() { return &front(); }

        const_pointer data() const { return &front(); }

        iterator begin() { return _start; }

        const_iterator begin() const { return _start; }

        const_iterator cbegin() const { return _start; }

        iterator end() { return _finish; }

        const_iterator end() const { return _finish; }

        const_iterator cend() const { return _finish; }

    //  reverse_iterator rbegin();
    //  const_reverse_iterator rbegin() const;
    //  reverse_iterator rend();
    //  const_reverse_iterator rend() const;

        bool empty() const { return begin() == end(); }
    
        size_type size() const { return end() - begin(); }

        size_type max_size() const { return _alloc.max_size(); }

        void reserve(size_type n); 

        size_type capacity() const { return _end_of_storage - _start; }

        void shrink_to_fit();

        void clear() { erase(begin(), end()); }

        iterator insert(const_iterator pos, const value_type& value) {
            return insert_aux(pos, value);
        }

        iterator insert(const_iterator pos, const value_type&& value) {
            return insert_aux(pos, HxSTL::move(value));
        }

        iterator insert(const_iterator pos, size_type count, const value_type& value) {
            if (count > 0) return insert_aux(pos, count, value, true_type());
            return pos;
        }

        template <class InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last) {
            if (first != last) return insert_aux(pos, first, last, typename is_integer<InputIterator>::value());
            return pos;
        }

        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args);

        iterator erase(iterator pos);

        iterator erase(iterator first, iterator last);

        void push_back(const value_type& value) { insert_aux(_finish, value); }

        void push_back(value_type&& value) { insert_aux(_finish, HxSTL::move(value)); }

        template <class... Args>
        void emplace_back(Args&&... args);

        void pop_back() { destroy(--_finish); }

        void resize(size_type count, value_type value = value_type());
        
        void swap(vector& other);
    public:
        template<class U, class A>
        friend bool operator==(const vector<U, A>& lhs, const vector<U, A>& rhs);
        template<class U, class A>
        friend bool operator!=(const vector<U, A>& lhs, const vector<U, A>& rhs);
        template<class U, class A>
        friend bool operator< (const vector<U, A>& lhs, const vector<U, A>& rhs);
        template<class U, class A>
        friend bool operator<=(const vector<U, A>& lhs, const vector<U, A>& rhs);
        template<class U, class A>
        friend bool operator> (const vector<U, A>& lhs, const vector<U, A>& rhs);
        template<class U, class A>
        friend bool operator>=(const vector<U, A>& lhs, const vector<U, A>& rhs);
    };

    template<class T, class Alloc>
    void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
        x.swap(y);
    }

    template <class T, class Alloc>
    template <class InputIterator>
    void vector<T, Alloc>::initialize_aux(InputIterator first, InputIterator last, false_type) {
        _start = _alloc.allocate(last - first);
        _finish = uninitialized_copy(first, last, _start);
        _end_of_storage = _finish;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::initialize_aux(size_type count, const value_type& value, true_type) {
        _start = _alloc.allocate(count);
        _finish = uninitialized_fill_n(_start, count, value);
        _end_of_storage = _start + count;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::initialize_aux(const vector& other) {
        _start = _alloc.allocate(other.capacity());
        _finish = uninitialized_copy(other.begin(), other.end(), _start);
        _end_of_storage = _start + other.capacity();
    }
    
    template <class T, class Alloc>
    void vector<T, Alloc>::initialize_aux(vector&& other) {
        _start = _alloc.allocate(other.capacity());
        _finish = uninitialized_move(other.begin(), other.end(), _start);
        _end_of_storage = _start + other.capacity();
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::resize(size_type n, value_type value) {
        if (n < size()) {
            destroy(_start + n, _finish);
            _finish = _start + n;
        } else if (n < capacity()) {
            _finish = uninitialized_fill_n(_finish, n - size(), value);
        } else {
            iterator new_start = _alloc.allocate(n);
            iterator new_finish = new_start;

            new_finish = uninitialized_move(_start, _finish, new_start);
            new_finish = uninitialized_fill_n(new_finish, n - size(), value);

            destroy_and_initialize(new_start, new_finish, new_finish);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::reserve(size_type n) {
        if (n > capacity()) {
            iterator new_start = _alloc.allocate(n);
            iterator new_finish = new_start;

            new_finish = uninitialized_move(_start, _finish, new_start);

            destroy_and_initialize(new_start, new_finish, new_start + n);
        }
    }

    template <class T, class Alloc>
    template <class InputIterator>
    inline void vector<T, Alloc>::assign_aux(InputIterator first, InputIterator last, false_type) {
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
    inline void vector<T, Alloc>::assign_aux(size_type n, const value_type& value, true_type) {
        if (n <= capacity()) {
            iterator old_finish = _finish;

            fill_n(_start, n, value);
            _finish = _start + n; 

            destroy(_finish, old_finish);
        } else {
            iterator new_start = _alloc.allocate(n);
            iterator new_finish = new_start;

            try {
                new_finish = uninitialized_fill_n(n, value, new_start);
            } catch (...) {
                destroy(new_start, new_finish);
                _alloc.deallocate(new_start, n);
                throw;
            }

            destroy_and_initialize(new_start, new_finish, new_finish);
        }
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert_aux(iterator pos, T&& value) {
        if (_finish != _end_of_storage) {
            if (pos == _finish) {
                _alloc.construct(_finish, HxSTL::forward<T>(value));
            } else {
                _alloc.construct(_finish, HxSTL::move(back()));
                copy_backward(pos, _finish - 1, _finish);
                *pos = HxSTL::forward<T>(value);
            }
            ++_finish;
            return pos;
        } else {
            allocate_and_single_insert(pos, HxSTL::forward<T>(value));
        }
    }

    template <class T, class Alloc>
    template <class InputIterator>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert_aux(iterator pos, InputIterator first, InputIterator last, false_type) {
        size_type count = last - first;

        if (_end_of_storage - _finish >= count) {
            size_type element_after = _finish - pos;
            iterator old_finish = _finish;

            if (count > element_after) {
                _finish = uninitialized_copy(first + element_after, last, _finish);
                _finish = uninitialized_move(pos, old_finish, _finish);
                copy_n(first, element_after, pos);
            } else {
                _finish = uninitialized_move(_finish - count, _finish, _finish);
                copy_backward(pos, old_finish - count, old_finish);
                copy(first, last, pos);
            }

            return pos;
        } else {
            size_type new_sz = 2 * capacity() > count ? 2 * capacity() : count;
            iterator new_start = _alloc.allocate(new_sz);
            iterator new_finish = new_start;

            new_finish = uninitialized_move(_start, pos, new_start);
            iterator result = new_finish;
            new_finish = uninitialized_copy(first, last, new_finish);
            new_finish = uninitialized_move(pos, _finish, new_finish);

            destroy_and_initialize(new_start, new_finish, new_start + new_sz);

            return result;
        }
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert_aux(iterator pos, size_type count, const value_type& value, true_type) {
        if (_end_of_storage - _finish >= count) {
            size_type element_after = _finish - pos;
            iterator old_finish = _finish;

            if (count > element_after) {
                _finish = uninitialized_fill_n(_finish, count - element_after, value);
                _finish = uninitialized_move(pos, old_finish, _finish);
                fill_n(pos, element_after, value);
            } else {
                _finish = uninitialized_move(_finish - count, _finish, _finish);
                copy_backward(pos, old_finish - count, old_finish);
                fill_n(pos, count, value);
            }

            return pos;
        } else {
            size_type new_sz = 2 * capacity() > count ? 2 * capacity(): count;

            iterator new_start = _alloc.allocate(new_sz);
            iterator new_finish = new_start;

            new_finish = uninitialized_move(_start, pos, new_start);
            iterator result = new_finish;
            new_finish = uninitialized_fill_n(new_finish, count, value);
            new_finish = uninitialized_move(pos, _finish, new_finish);

            destroy_and_initialize(new_start, new_finish, new_start + new_sz);

            return result;
        }
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos) {
        _finish = move(pos + 1, _finish, pos);
        _alloc.destroy(_finish);
        return pos;
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
        iterator old_finish = _finish;
        _finish = move(last, _finish, first);
        HxSTL::destroy(_finish, old_finish);
        return first;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::swap(vector& other) {
        HxSTL::swap(_start, other._start);
        HxSTL::swap(_finish, other._finish);
        HxSTL::swap(_end_of_storage, other._end_of_storage);
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::allocate_and_single_insert(iterator pos, T&& value) {
        size_type new_sz = size() ? 2 * size() : 1;

        iterator new_start = _alloc.allocate(new_sz);
        iterator new_finish = new_start;

        new_finish = uninitialized_move(_start, pos, new_start);
        iterator result = new_finish;
        _alloc.construct(new_finish++, HxSTL::forward<T>(value));
        new_finish = uninitialized_move(pos, _finish, new_finish);

        destroy_and_initialize(new_start, new_finish, new_start + new_sz);

        return result;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::destroy_and_initialize(iterator new_start, 
            iterator new_finish, iterator new_end_of_storage) {
        if (_start) {
            _alloc.deallocate(_start, capacity());
        }

        _start = new_start;
        _finish = new_finish;
        _end_of_storage = new_end_of_storage;
    }

    template<class U, class A>
    bool operator==(const vector<U, A>& lhs, const vector<U, A>& rhs) {
        // Todo
    }

    template<class U, class A>
    bool operator!=(const vector<U, A>& lhs, const vector<U, A>& rhs) {
        return !(lhs == rhs);
    }
    
    template<class U, class A>
    bool operator< (const vector<U, A>& lhs, const vector<U, A>& rhs) {
        // Todo
    }

    template<class U, class A>
    bool operator<=(const vector<U, A>& lhs, const vector<U, A>& rhs) {
        return !(lhs < rhs);
    }

    template<class U, class A>
    bool operator> (const vector<U, A>& lhs, const vector<U, A>& rhs) {
        return rhs < lhs;
    }

    template<class U, class A>
    bool operator>=(const vector<U, A>& lhs, const vector<U, A>& rhs) {
        return !(rhs < lhs);
    }

}


#endif
