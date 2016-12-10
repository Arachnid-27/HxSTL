#ifndef _DEQUE_H_
#define _DEQUE_H_


#include "allocator.h"
#include "uninitialized.h"
#include "utility.h"


namespace HxSTL {

    template <class T, class Ref, class Ptr>
    class deque_iterator {
    protected:
        enum {__BUFFER_BYTE = 512};
    public:
        typedef bidirectional_iterator_tag          iterator_category;
        typedef T                                   value_type;
        typedef Ptr                                 pointer;
        typedef Ref                                 reference;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;

        typedef T*                                  ele_pointer;
        typedef T**                                 map_pointer;
    public:
        ele_pointer _cur;
        ele_pointer _first;
        ele_pointer _last;
        map_pointer _node;
    public:
        constexpr static size_t buffer_size() { return __BUFFER_BYTE / sizeof(T); }

        void set_node(map_pointer node) noexcept {
            _node = node;
            _first = *node;
            _last = _first + buffer_size();
        }
    public:
        deque_iterator() noexcept
            : _cur(nullptr), _first(nullptr), _last(nullptr), _node(nullptr) {}

        deque_iterator(ele_pointer cur, map_pointer node) noexcept
            : _cur(cur), _first(*node), _last(*node + buffer_size()), _node(node) {}

        deque_iterator(const deque_iterator<T, T&, T*>& other) noexcept
            : _cur(other._cur), _first(other._first), _last(other._last), _node(other._node) {}

        reference operator*() const noexcept { return *_cur; }

        pointer operator->() const noexcept { return _cur; }

        deque_iterator& operator++() {
            ++_cur;
            if (_cur == _last) {
                set_node(_node + 1);
                _cur = _first;
            }
            return *this;
        }

        deque_iterator operator++(int) {
            deque_iterator it = *this;
            ++*this;
            return it;
        }

        deque_iterator& operator--() noexcept {
            if (_cur == _first) {
                set_node(_node - 1);
                _cur = _last;
            }
            --_cur;
            return *this;
        }

        deque_iterator operator--(int) noexcept {
            deque_iterator tmp = *this;
            --*this;
            return tmp;
        }

        deque_iterator& operator+=(difference_type n) noexcept {
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

        deque_iterator operator+(difference_type n) const noexcept {
            deque_iterator it = *this;
            return it += n;
        }

        deque_iterator& operator-=(difference_type n) noexcept {
            return *this += -n;
        }

        deque_iterator operator-(difference_type n) const noexcept {
            deque_iterator it = *this;
            return it -= n;
        }

        reference operator[](difference_type n) const noexcept {
            return *(*this + n);
        }
    };

    template <class T, class Ref, class Ptr>
    inline bool operator==(const deque_iterator<T, Ref, Ptr>& x, const deque_iterator<T, Ref, Ptr>& y) noexcept {
        return x._cur == y._cur;
    }

    template <class T, class Ref, class Ptr>
    inline bool operator!=(const deque_iterator<T, Ref, Ptr>& x, const deque_iterator<T, Ref, Ptr>& y) noexcept {
        return x._cur != y._cur;
    }

    template <class T, class Ref, class Ptr>
    inline bool operator< (const deque_iterator<T, Ref, Ptr>& x, const deque_iterator<T, Ref, Ptr>& y) noexcept {
        return x._cur < y._cur;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    inline bool operator==(const deque_iterator<T, RefL, PtrL>& x, const deque_iterator<T, RefR, PtrR>& y) noexcept {
        return x._cur == y._cur;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    inline bool operator!=(const deque_iterator<T, RefL, PtrL>& x, const deque_iterator<T, RefR, PtrR>& y) noexcept {
        return x._cur != y._cur;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    inline bool operator< (const deque_iterator<T, RefL, PtrL>& x, const deque_iterator<T, RefR, PtrR>& y) noexcept {
        return x._cur < y._cur;
    }

    template <class T, class Ref, class Ptr>
    inline typename deque_iterator<T, Ref, Ptr>::difference_type
    operator- (const deque_iterator<T, Ref, Ptr>& x, const deque_iterator<T, Ref, Ptr>& y) noexcept {
        return x.buffer_size() * (x._node - y._node - 1) + (x._cur - x._first) + (y._last - y._cur);
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    inline typename deque_iterator<T, RefL, PtrL>::difference_type
    operator- (const deque_iterator<T, RefL, PtrL>& x, const deque_iterator<T, RefR, PtrR>& y) noexcept {
        return x.buffer_size() * (x._node - y._node - 1) + (x._cur - x._first) + (y._last - y._cur);
    }

    template <class T, class Alloc = allocator<T>>
    class deque {
    public:
        typedef T                                                       value_type;
        typedef Alloc                                                   allocator_type;
        typedef value_type&                                             reference;
        typedef const value_type&                                       const_reference;
        typedef value_type*                                             pointer;
        typedef const value_type*                                       const_pointer;
        typedef deque_iterator<T, T&, T*>                               iterator;
        typedef deque_iterator<T, const T&, const T*>                   const_iterator;
    //  typedef reverse_iterator<iterator>                              reverse_iterator;
    //  typedef reverse_iterator<const_iterator>                        const_reverse_iterator;
        typedef ptrdiff_t                                               difference_type;
        typedef size_t                                                  size_type;

        typedef typename iterator::map_pointer                          map_pointer;
        typedef typename Alloc::template rebind<pointer>::other         map_allocator_type;
    protected:
        allocator_type _alloc;
        map_allocator_type _map_alloc; 
        iterator _start;
        iterator _finish;
        map_pointer _map;
        size_type _map_size;
    protected:
        template <class InputIt>
        void initialize_aux(InputIt first, InputIt last, HxSTL::false_type);
        void initialize_aux(size_type count, const T& value, HxSTL::true_type);
        template <class InputIt>
        void assign_aux(InputIt first, InputIt last, HxSTL::false_type);
        void assign_aux(size_type count, const T& value, HxSTL::true_type);
        template <class Y>
        iterator insert_aux(iterator pos, Y&& value);
        template <class InputIt>
        iterator insert_aux(iterator pos, InputIt first, InputIt last, HxSTL::false_type);
        iterator insert_aux(iterator pos, size_type count, const T& value, HxSTL::true_type);
        void alloc_node(size_type count, bool alloc_at_front);
        void create_map(size_type count);
        void reallocate_map(size_type add_num_nodes, bool add_at_front);
        iterator REMOVE_CONST(const_iterator it) noexcept { return iterator(it._cur, it._node); }
    public:
        explicit deque(const Alloc& alloc = Alloc())
            : _alloc(alloc), _map_alloc(map_allocator_type()) {
                create_map(0);
            }

        explicit deque(size_type count, const T& val, const Alloc& alloc = Alloc())
            : _alloc(alloc), _map_alloc(map_allocator_type()) {
                initialize_aux(count, val, HxSTL::true_type());
            }

        explicit deque(size_type count)
            : _alloc(Alloc()), _map_alloc(map_allocator_type()) {
                initialize_aux(count, T(), HxSTL::true_type());
            }

        template <class InputIt>
        deque(InputIt first, InputIt last, const Alloc& alloc = Alloc())
            : _alloc(alloc), _map_alloc(map_allocator_type()) {
                initialize_aux(first, last, typename HxSTL::is_integeral<InputIt>::type());
            }

        deque(const deque& other): _alloc(other._alloc), _map_alloc(other._map_alloc) {
            initialize_aux(other._start, other._finish, HxSTL::false_type());
        }

        deque(const deque& other, const Alloc& alloc): _alloc(alloc), _map_alloc(map_allocator_type()) {
            initialize_aux(other._start, other._finish, HxSTL::false_type());
        }

        deque(deque&& other): _start(other._start), _finish(other._finish), _map(other._map), 
        _map_size(other._map_size), _alloc(HxSTL::move(other._alloc)), _map_alloc(HxSTL::move(other._map_alloc)) {
            other._map = nullptr;
            other._map_size = 0;
        }

        deque(deque&& other, const Alloc& alloc): _start(other._start), _finish(other._finish), _map(other._map), 
        _map_size(other._map_size), _alloc(alloc), _map_alloc(map_allocator_type()) {
            other._map = nullptr;
            other._map_size = 0;
        }

        deque(HxSTL::initializer_list<T> init, const Alloc& alloc = Alloc())
            : _alloc(alloc), _map_alloc(map_allocator_type()) {
                initialize_aux(init.begin(), init.end(), HxSTL::false_type());
            }

        ~deque() {
            if (_map) {
                HxSTL::destroy(_start, _finish);
                for (map_pointer mp = _start._node; mp <= _finish._node; ++mp) {
                    _alloc.deallocate(*mp, iterator::buffer_size());
                }
                _map_alloc.deallocate(_map, _map_size);
            }
        }

        deque& operator=(const deque& other) {
            deque(other).swap(*this);
            return *this;
        }

        deque& operator=(deque&& other) {
            deque(HxSTL::move(other)).swap(*this);
            return *this;
        }

        deque& operator=(HxSTL::initializer_list<T> init) {
            deque(init).swap(*this);
            return *this;
        }

        void assign(size_type count, const T& value) {
            assign_aux(count, value, HxSTL::false_type());
        }

        template <class InputIt>
        void assign(InputIt first, InputIt last) {
            assign_aux(first, last, typename HxSTL::is_integeral<InputIt>::type());
        }

        void assign(HxSTL::initializer_list<T> init) {
            assign_aux(init.begin(), init.end(), HxSTL::false_type());
        }

        allocator_type get_allocator() const { return _alloc; }

        reference at(size_type pos) {
            if (pos >= size()) {
                throw HxSTL::out_of_range();
            }
            return operator[](pos);
        }

        const_reference at(size_type pos) const {
            if (pos >= size()) {
                throw HxSTL::out_of_range();
            }
            return operator[](pos);
        }

        reference operator[](size_type pos) { return *(begin() + pos); }

        const_reference operator[](size_type pos) const { return *(begin() + pos); }
 
        reference front() { return *begin(); }

        const_reference front() const { return *begin(); }

        reference back() { return *(end() - 1); }

        const_reference back() const { return *(end() - 1); }

        iterator begin() { return _start; }

        const_iterator begin() const { return _start; }

        const_iterator cbegin() const { return _start; }

        iterator end() { return _finish; }

        const_iterator end() const { return _finish; }

        const_iterator cend() const { return _finish; }

    //  reverse_iterator rbegin();

    //  const_reverse_iterator rbegin() const;

    //  const_reverse_iterator crbegin() const;

    //  reverse_iterator rend();

    //  const_reverse_iterator rend() const;
    
    //  const_reverse_iterator crend() const;

        bool empty() const { return begin() == end(); }

        size_type size() const { return end() - begin(); }

        size_type max_size() const { return _alloc.max_size(); }

        void clear();
    
        iterator insert(const_iterator pos, const T& value) {
            return insert_aux(REMOVE_CONST(pos), value);
        }

        iterator insert(const_iterator pos, T&& value) {
            return insert_aux(REMOVE_CONST(pos), HxSTL::move(value));
        }

        iterator insert(const_iterator pos, size_type count, const T& value) {
            if (count != 0) {
                return insert_aux(REMOVE_CONST(pos), count, value, HxSTL::true_type());
            }
            return REMOVE_CONST(pos);
        }

        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            if (first != last) {
                return insert_aux(REMOVE_CONST(pos), first, last, typename HxSTL::is_integeral<InputIt>::type());
            }
            return REMOVE_CONST(pos);
        }

        iterator insert(const_iterator pos, HxSTL::initializer_list<T> init) {
            if (!init.empty()) {
                return insert_aux(REMOVE_CONST(pos), init.begin(), init.end(), HxSTL::false_type());
            }
            return REMOVE_CONST(pos);
        }
 
        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args);

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);

        void push_back(const T& value) { insert_aux(end(), value); }

        void push_back(T&& value) { insert_aux(end(), HxSTL::move(value)); }

        template <class... Args>
        void emplace_back(Args&&... args);

        void pop_back();

        void push_front(const T& value) { insert_aux(begin(), value); }

        void push_front(T&& value) { insert_aux(begin(), HxSTL::move(value)); }

        template <class... Args>
        void emplace_front(Args&&... args);

        void pop_front();

        void resize(size_type count);

        void resize(size_type count, const value_type& value);

        void swap(deque& other) {
            HxSTL::swap(_start, other._start);
            HxSTL::swap(_finish, other._finish);
            HxSTL::swap(_map, other._map);
            HxSTL::swap(_map_size, other._map_size);
        }
    };

    template <class T, class Alloc>
    template <class InputIt>
    void deque<T, Alloc>::initialize_aux(InputIt first, InputIt last, HxSTL::false_type) {
        create_map(HxSTL::distance(first, last));
        HxSTL::uninitialized_copy(first, last, _start);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::initialize_aux(size_type count, const T& value, HxSTL::true_type) {
        create_map(count);
        HxSTL::uninitialized_fill_n(_start, count, value);
    }

    template <class T, class Alloc>
    template <class InputIt>
    void deque<T, Alloc>::assign_aux(InputIt first, InputIt last, HxSTL::false_type) {
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::assign_aux(size_type count, const T& value, HxSTL::true_type) {
    }

    template <class T, class Alloc>
    template <class Y>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert_aux(iterator pos, Y&& value) {
        if (pos - _start < size() / 2) {
            if (_start._node == _map && _start._cur == _start._first) {
                reallocate_map(1, true);
            }
            if (_start._cur == _start._first) {
                *(_start._node - 1) = _alloc.allocate(iterator::buffer_size());
            }
            if (pos == _start) {
                _alloc.construct(&*(--_start), HxSTL::forward<Y>(value));
                return _start;
            } else {
                _alloc.construct(&*(_start - 1), front());
                HxSTL::copy(_start + 1, pos, _start);
                *(--pos) = HxSTL::forward<T>(value);
                --_start;
            }
        } else {
            if (_finish._cur == _finish._last - 1 && _finish._node == _map + _map_size) {
                reallocate_map(1, false);
            }
            if (_finish._cur == _finish._last - 1) {
                *(_finish._node + 1) = _alloc.allocate(iterator::buffer_size());
            }
            if (pos == _finish) {
                _alloc.construct(&*(_finish++), HxSTL::forward<Y>(value));
            } else {
                _alloc.construct(&*(_finish + 1), back());
                HxSTL::copy_backward(pos, _finish, _finish + 1);
                *pos = HxSTL::forward<T>(value);
                ++_finish;
            }
        }
        return pos;
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::alloc_node(size_type count, bool alloc_at_front) {
        size_type remain_element = alloc_at_front ? _start._cur - _start._first : _finish._last - _finish._cur;
        if (remain_element < count) {
            size_type add_num_nodes = (count - remain_element - 1) / iterator::buffer_size() + 1;

            size_type remain_map = alloc_at_front ? _start._node - _map : _map + _map_size - _finish._node;
            size_type remain = remain_element + iterator::buffer_size() * remain_map;
            if (remain < count) {
                reallocate_map(add_num_nodes, alloc_at_front);
            }

            if (alloc_at_front) {
                map_pointer mp = _start._node - 1;
                for (size_type i = 0; i != add_num_nodes; ++i) {
                    mp[-i] = _alloc(iterator::buffer_size());
                }
            } else {
                map_pointer mp = _finish._node + 1;
                for (size_type i = 0; i != add_num_nodes; ++i) {
                    mp[i] = _alloc(iterator::buffer_size());
                }
            }
        }
    }

    template <class T, class Alloc>
    template <class InputIt>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert_aux(iterator pos, InputIt first, InputIt last, HxSTL::false_type) {
        size_type count = HxSTL::distance(first, last);
        size_type index = pos - _start;
        if (index < size() / 2) {
            alloc_node(count, true);

            iterator old_start = _start;
            _start -= count;
            if (pos == _start) {
                HxSTL::uninitialized_copy(first, last, _start);
                return _start;
            } else if (index < count) {
                HxSTL::uninitialized_copy_n(old_start, index, _start);
                HxSTL::uninitialized_copy_n(first, count - index, _start + index);
                HxSTL::advance(first, count - index);
                HxSTL::copy(first, last, old_start);
                return _start + index;
            } else {
                HxSTL::uninitialized_copy_n(old_start, count, _start);
                HxSTL::copy_n(old_start + count, index - count, old_start);
                HxSTL::copy(first, last, old_start + count);
                return old_start + count;
            }
        } else {
            size_type element_after = _finish - pos;

            alloc_node(count, false);

            iterator old_finish = _finish;
            if (element_after < count) {
                _finish = HxSTL::uninitialized_copy(_finish - element_after, _finish, _finish + count - element_after);
                HxSTL::copy_n(first, element_after, old_finish - element_after);
                HxSTL::advance(first, element_after);
                HxSTL::uninitialized_copy(first, last, old_finish);
                return old_finish - element_after;
            } else {
                _finish = HxSTL::uninitialized_copy(_finish - count, _finish, _finish);
                HxSTL::copy_backward(pos, old_finish - count, old_finish);
                HxSTL::copy(first, last, pos);
                return old_finish - element_after;
            }
        }
    }

    template <class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert_aux(iterator pos, size_type count, const T& value, HxSTL::true_type) {
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::create_map(size_type count) {
        size_type num_nodes = count / iterator::buffer_size() + 1;

        _map_size = num_nodes > 6 ? num_nodes + 2 : 8;
        _map = _map_alloc.allocate(_map_size);   

        map_pointer start = _map + (_map_size - num_nodes) / 2;
        map_pointer finish = start + num_nodes - 1;

        for (map_pointer mp = start; mp <= finish; ++mp) {
            *mp = _alloc.allocate(iterator::buffer_size());
        }

        _start.set_node(start);
        _finish.set_node(finish);
        _start._cur = _start._first;
        _finish._cur = _finish._first + count % iterator::buffer_size();
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::reallocate_map(size_type add_num_nodes, bool add_at_front) {
        size_type old_num_nodes = _finish._node - _start._node + 1;
        size_type new_num_nodes = old_num_nodes + add_num_nodes;

        map_pointer new_start;

        if (_map_size > 2 * new_num_nodes) {
            new_start = _map + (_map_size - new_num_nodes) / 2 + (add_at_front ? add_num_nodes : 0);
            if (new_start < _start._node) {
                HxSTL::copy(_start._node, _finish._node + 1, new_start);
            } else {
                HxSTL::copy_backward(_start._node, _finish._node + 1, new_start + old_num_nodes);
            }
        } else {
            size_type new_map_size = _map_size + (_map_size > add_num_nodes ? _map_size : add_num_nodes) + 2;
            map_pointer new_map = _map_alloc.allocate(new_map_size);
            new_start = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? add_num_nodes : 0);

            HxSTL::copy(_start._node, _finish._node + 1, new_start); // 可以不用 uninitialized_copy
            _map_alloc.deallocate(_map, _map_size);

            _map = new_map;
            _map_size = new_map_size;
        }

        _start._node = new_start;
        _finish._node = new_start + old_num_nodes - 1;
    }

    template <class T, class Alloc>
    bool operator==(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs) {
        if (lhs.size() != lhs.size()) return false;

        for (auto first1 = lhs.begin(), last1 = lhs.end(), first2 = rhs.begin(); first1 != last1; ++first1, ++first2) {
            if (*first1 != *first2) return false;
        }
        return true;
    }

    template <class T, class Alloc>
    bool operator!=(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs) {
        return !(lhs == rhs);
    }

}


#endif
