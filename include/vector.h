#ifndef _VECTOR_H_
#define _VECTOR_H_


#include "allocator.h"
#include "uninitialized.h"
#include "stdexcept.h"


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
        template <class InputIt>
        void initialize_aux(InputIt first, InputIt last, false_type);
        void initialize_aux(size_type count, const value_type& value, true_type);
        void initialize_aux(const vector& other);
        void initialize_aux(vector&& other);
        template <class InputIt>
        iterator insert_aux(iterator pos, InputIt first, InputIt last, false_type);
        iterator insert_aux(iterator pos, size_type count, const T& value, true_type);
        template <class U>
        iterator insert_aux(iterator pos, U&& value);
        template <class... Args>
        iterator emplace_aux(iterator pos, Args&&... args);
        iterator erase_aux(iterator pos);
        iterator erase_aux(iterator first, iterator last);
        template <class InputIt>
        void assign_aux(InputIt first, InputIt last, false_type);
        void assign_aux(size_type count, const T& value, true_type);
        void resize_aux(size_type count, const T& value);
        void destroy_and_reset(iterator new_start, iterator new_finish, iterator new_end_of_storage);
    public:
        vector(): _start(nullptr), _finish(nullptr), _end_of_storage(nullptr), _alloc(Alloc()) {}

        explicit vector(const Alloc& alloc): _start(nullptr), _finish(nullptr), _end_of_storage(nullptr), _alloc(alloc) {}

        explicit vector(size_type count, const T& value, const Alloc& alloc = Alloc())
            : _alloc(alloc) { initialize_aux(count, value, HxSTL::true_type()); }

        explicit vector(size_type count, const Alloc& alloc = Alloc()) 
            : _alloc(alloc) { initialize_aux(count, T(), HxSTL::true_type()); }

        template <class InputIt>
        vector(InputIt first, InputIt last, const Alloc& alloc = Alloc())
            : _alloc(alloc) { initialize_aux(first, last, typename HxSTL::is_integeral<InputIt>::type()); }

        vector(const vector& other): _alloc(other._alloc) { initialize_aux(other); }

        vector(const vector& other, const Alloc& alloc): _alloc(alloc) { initialize_aux(other); }

        vector(vector&& other): _alloc(HxSTL::move(other._alloc)) { initialize_aux(HxSTL::move(other)); }

        vector(vector&& other, const Alloc& alloc): _alloc(alloc) { initialize_aux(HxSTL::move(other)); }

        ~vector() {
            if (_start) {
                HxSTL::destroy(_alloc, _start, _finish);
                _alloc.deallocate(_start, capacity());
            }
        }
        
        vector& operator=(const vector& other) {
            assign(other.begin(), other.end());
            return *this;
        }

        vector& operator=(vector&& other) {
            vector(HxSTL::move(other)).swap(*this);
            return *this;
        }

        template <class InputIt>
        void assign(InputIt first, InputIt last) {
            assign_aux(first, last, typename HxSTL::is_integeral<InputIt>::type());
        }

        void assign(size_type n, const value_type& value) {
            assign_aux(n, value, HxSTL::true_type());
        }

        allocator_type get_allocator() const { return _alloc; }

        reference at(size_type n) { 
            if (n >= size()) {
                throw HxSTL::out_of_range();
            }
            return operator[](n);
        }

        const_reference at(size_type n) const {
            if (n >= size()) {
                throw HxSTL::out_of_range();
            }
            return operator[](n);
        }

        reference operator[](size_type n) { return *(begin() + n); }

        const_reference operator[](size_type n) const { return *(begin() + n); }

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

        void reserve(size_type new_cap); 

        size_type capacity() const { return _end_of_storage - _start; }

        void shrink_to_fit();

        void clear() { erase_aux(begin(), end()); }

        iterator insert(const_iterator pos, const T& value) {
            return insert_aux(begin() + (pos - cbegin()), value);
        }

        iterator insert(const_iterator pos, T&& value) {
            return insert_aux(begin() + (pos - cbegin()), HxSTL::move(value));
        }

        iterator insert(const_iterator pos, size_type count, const T& value) {
            return insert_aux(begin() + (pos - cbegin()), count, value, HxSTL::true_type());
        }

        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            return insert_aux(begin() + (pos - cbegin()), first, last, typename HxSTL::is_integeral<InputIt>::type());
        }

        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args) {
            return emplace_aux(begin() + (pos - cbegin()), HxSTL::forward<Args>(args)...);
        }

        iterator erase(const_iterator pos) {
            return erase_aux(begin() + (pos - cbegin()));
        }

        iterator erase(const_iterator first, const_iterator last) {
            return erase_aux(begin() + (first - cbegin()), begin() + (last - cbegin()));
        }

        void push_back(const T& value) {
            insert_aux(end(), value);
        }

        void push_back(T&& value) {
            insert_aux(end(), HxSTL::move(value));
        }

        template <class... Args>
        void emplace_back(Args&&... args) {
            emplace_aux(end(), HxSTL::forward<Args>(args)...);
        }

        void pop_back() { _alloc.destroy(--_finish); }

        void resize(size_type count) { resize_aux(count, T()); }

        void resize(size_type count, const T& value) { resize_aux(count, value); }
        
        void swap(vector& other);
    };

    template<class T, class Alloc>
    void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
        x.swap(y);
    }

    template <class T, class Alloc>
    template <class InputIt>
    void vector<T, Alloc>::initialize_aux(InputIt first, InputIt last, false_type) {
        _start = _alloc.allocate(HxSTL::distance(first, last));
        _finish = HxSTL::uninitialized_copy(first, last, _start);
        _end_of_storage = _finish;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::initialize_aux(size_type count, const T& value, true_type) {
        _start = _alloc.allocate(count);
        _finish = HxSTL::uninitialized_fill_n(_start, count, value);
        _end_of_storage = _finish;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::initialize_aux(const vector& other) {
        _start = _alloc.allocate(other.size());
        _finish = HxSTL::uninitialized_copy(other.begin(), other.end(), _start);
        _end_of_storage = _finish;
    }
    
    template <class T, class Alloc>
    void vector<T, Alloc>::initialize_aux(vector&& other) {
        _start = other._start;
        _finish = other._finish;
        _end_of_storage = other._end_of_storage;
        other._start = other._finish = other._end_of_storage = nullptr;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::resize_aux(size_type count, const T& value) {
        if (count <= size()) {
            // 删除多余元素
            HxSTL::destroy(_alloc, _start + count, _finish);
            _finish = _start + count;
        } else if (count <= capacity()) {
            // 填充元素
            _finish = uninitialized_fill_n(_finish, count - size(), value);
        } else {
            iterator new_start = _alloc.allocate(count);
            iterator new_finish = new_start;
            new_finish = uninitialized_copy(_start, _finish, new_start);
            new_finish = uninitialized_fill_n(new_finish, count - size(), value);
            destroy_and_reset(new_start, new_finish, new_finish);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::reserve(size_type new_cap) {
        if (new_cap > max_size()) {
            throw HxSTL::length_error();
        } else if (new_cap > capacity()) {
            iterator new_start = _alloc.allocate(new_cap);
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            destroy_and_reset(new_start, new_finish, new_start + new_cap);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::shrink_to_fit() {
        if (_end_of_storage != _finish) {
            iterator new_start = _alloc.allocate(size());
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            destroy_and_reset(new_start, new_finish, new_start + size());
        }
    }

    template <class T, class Alloc>
    template <class InputIt>
    void vector<T, Alloc>::assign_aux(InputIt first, InputIt last, false_type) {
        size_type count = HxSTL::distance(first, last);
        if (count <= size()) {
            // 拷贝并删除多余元素
            iterator old_finish = _finish;
            _finish = HxSTL::copy(first, last, _start);
            HxSTL::destroy(_alloc, _finish, old_finish);
        } else if (count <= capacity()) {
            // 分开拷贝已初始化和未初始化区域
            size_type sz = size();
            HxSTL::copy_n(first, sz, _start);
            HxSTL::advance(first, sz);
            _finish = HxSTL::uninitialized_copy(first, last, _finish);
        } else {
            iterator new_start = _alloc.allocate(count);
            iterator new_finish = HxSTL::uninitialized_copy(first, last, new_start);
            destroy_and_reset(new_start, new_finish, new_finish);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::assign_aux(size_type count, const value_type& value, true_type) {
        if (count <= size()) {
            // 填充并删除多余元素
            iterator old_finish = _finish;
            _finish = HxSTL::fill_n(_start, count, value);
            HxSTL::destroy(_alloc, _finish, old_finish);
        } else if (count <= capacity()) {
            // 分开填充已初始化和未初始化区域
            size_type sz = size();
            HxSTL::fill_n(_start, sz, value);
            _finish = HxSTL::uninitialized_fill_n(_finish, count - sz, value);
        } else {
            iterator new_start = _alloc.allocate(count);
            iterator new_finish = HxSTL::uninitialized_fill_n(new_start, count, value);
            destroy_and_reset(new_start, new_finish, new_finish);
        }
    }

    template <class T, class Alloc>
    template <class U>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert_aux(iterator pos, U&& value) {
        if (_finish != _end_of_storage) {
            // 预留空间足够
            if (pos == _finish) {
                // 末尾插入
                _alloc.construct(_finish, HxSTL::forward<U>(value));
            } else {
                // 非末尾插入
                _alloc.construct(_finish, back());
                HxSTL::copy_backward(pos, _finish - 1, _finish);
                *pos = HxSTL::forward<U>(value);
            }
            ++_finish;
            return pos;
        } else {
            size_type new_sz = size() ? 2 * size() : 1;
            iterator new_start = _alloc.allocate(new_sz);
            iterator new_finish = HxSTL::uninitialized_copy(_start, pos, new_start);
            iterator result = new_finish;
            _alloc.construct(new_finish++, HxSTL::forward<U>(value));
            new_finish = HxSTL::uninitialized_copy(pos, _finish, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
            return result;
        }
    }

    template <class T, class Alloc>
    template <class... Args>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::emplace_aux(iterator pos, Args&&... args) {
        if (_finish != _end_of_storage) {
            // 预留空间足够
            if (pos == _finish) {
                // 末尾插入
                _alloc.construct(_finish, HxSTL::forward<Args>(args)...);
            } else {
                // 非末尾插入
                _alloc.construct(_finish, back());
                HxSTL::copy_backward(pos, _finish - 1, _finish);
                _alloc.destroy(pos);
                _alloc.construct(pos, HxSTL::forward<Args>(args)...);
            }
            ++_finish;
            return pos;
        } else {
            size_type new_sz = size() ? 2 * size() : 1;
            iterator new_start = _alloc.allocate(new_sz);
            iterator new_finish = HxSTL::uninitialized_copy(_start, pos, new_start);
            iterator result = new_finish;
            _alloc.construct(new_finish++, HxSTL::forward<Args>(args)...);
            new_finish = HxSTL::uninitialized_copy(pos, _finish, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
            return result;
        }
    }

    template <class T, class Alloc>
    template <class InputIt>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert_aux(iterator pos, InputIt first, InputIt last, false_type) {
        if (first == last) return pos;

        size_type count = HxSTL::distance(first, last);
        if (_end_of_storage - _finish >= count) {
            // 预留空间足够
            size_type element_after = _finish - pos;
            iterator old_finish = _finish;
            if (count > element_after) {
                // 插入点之后元素少于待插入元素
                _finish = HxSTL::uninitialized_copy(pos, _finish, _finish + count - element_after);
                copy_n(first, element_after, pos);
                HxSTL::advance(first, element_after);
                HxSTL::uninitialized_copy(first, last, old_finish);
            } else {
                // 插入点之后元素不少于待插入元素
                _finish = HxSTL::uninitialized_copy(_finish - count, _finish, _finish);
                HxSTL::copy_backward(pos, old_finish - count, old_finish);
                HxSTL::copy(first, last, pos);
            }
            return pos;
        } else {
            size_type new_sz = 2 * capacity() > size() + count ? 2 * capacity() : size() + count;
            iterator new_start = _alloc.allocate(new_sz);
            iterator new_finish = HxSTL::uninitialized_copy(_start, pos, new_start);
            iterator result = new_finish;
            new_finish = HxSTL::uninitialized_copy(first, last, new_finish);
            new_finish = HxSTL::uninitialized_copy(pos, _finish, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
            return result;
        }
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert_aux(iterator pos, size_type count, const value_type& value, HxSTL::true_type) {
        if (count == 0) return pos;

        if (_end_of_storage - _finish >= count) {
            // 预留空间足够
            size_type element_after = _finish - pos;
            iterator old_finish = _finish;
            if (count > element_after) {
                // 插入点之后元素少于待插入元素
                _finish = HxSTL::uninitialized_fill_n(_finish, count - element_after, value);
                _finish = HxSTL::uninitialized_copy(pos, old_finish, _finish);
                HxSTL::fill_n(pos, element_after, value);
            } else {
                // 插入点之后元素不少于待插入元素
                _finish = HxSTL::uninitialized_copy(_finish - count, _finish, _finish);
                _finish = HxSTL::copy_backward(pos, old_finish - count, old_finish);
                HxSTL::fill_n(pos, count, value);
            }
            return pos;
        } else {
            size_type new_sz = 2 * capacity() > count + size() ? 2 * capacity(): count + size();
            iterator new_start = _alloc.allocate(new_sz);
            iterator new_finish = HxSTL::uninitialized_copy(_start, pos, new_start);
            iterator result = new_finish;
            new_finish = HxSTL::uninitialized_fill_n(new_finish, count, value);
            new_finish = HxSTL::uninitialized_copy(pos, _finish, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
            return result;
        }
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase_aux(iterator pos) {
        _finish = HxSTL::copy(pos + 1, _finish, pos);
        _alloc.destroy(_finish);
        return pos;
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase_aux(iterator first, iterator last) {
        iterator old_finish = _finish;
        _finish = HxSTL::copy(last, _finish, first);
        HxSTL::destroy(_alloc, _finish, old_finish);
        return first;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::swap(vector& other) {
        HxSTL::swap(_start, other._start);
        HxSTL::swap(_finish, other._finish);
        HxSTL::swap(_end_of_storage, other._end_of_storage);
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::destroy_and_reset(iterator new_start, 
            iterator new_finish, iterator new_end_of_storage) {
        if (_start) {
            HxSTL::destroy(_alloc, _start, _finish);
            _alloc.deallocate(_start, capacity());
        }

        _start = new_start;
        _finish = new_finish;
        _end_of_storage = new_end_of_storage;
    }

    template<class U, class A>
    bool operator==(const vector<U, A>& lhs, const vector<U, A>& rhs) {
        if (lhs.size() != rhs.size()) return false;

        auto first1 = lhs.cbegin();
        auto last1 = lhs.cend();
        auto first2 = rhs.cbegin();
        auto last2 = rhs.cend();
        while (first1 != last1) {
            if (first1 != first2) return false; 
            ++first1;
            ++first2;
        }
        return true;
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
