#ifndef _BASIC_STRING_
#define _BASIC_STRING_


#include "allocator.h"
#include "uninitialized.h"


namespace HxSTL {

    template <class CharT, class Alloc = allocator<CharT>>
    class basic_string {
        enum { DEFAULT_SIZE = 15 };
    public:
        typedef CharT                                       value_type;
        typedef Alloc                                       allocator_type;
        typedef typename Alloc::size_type                   size_type;
        typedef typename Alloc::difference_type             difference_type;
        typedef typename Alloc::reference                   reference;
        typedef typename Alloc::const_reference             const_reference;
        typedef typename Alloc::pointer                     pointer;
        typedef typename Alloc::const_pointer               const_pointer;
        typedef pointer                                     iterator;
        typedef const_pointer                               const_iterator;
//      typedef reverse_iterator<iterator>                  reverse_iterator;
//      typedef reverse_iterator<const_iterator>            const_reverse_iterator;
    public:
        static const size_type npos = -1;
    protected:
        long* _ref_count;
        iterator _start;
        iterator _finish;
        iterator _end_of_storage;
        allocator_type _alloc;
    protected:
        template <class InputIt>
        void initialize_aux(InputIt first, InputIt last, false_type);
        void initialize_aux(size_type count, CharT ch, true_type);
        void initialize_aux(const_iterator first, size_type count);
        template <class InputIt>
        void assign_aux(InputIt first, InputIt last, false_type);
        void assign_aux(size_type count, CharT ch, true_type);
        void assign_aux(const_iterator first, size_type count);
        void insert_char_string_aux(size_type index, iterator first, iterator last);
        void reallocate_and_copy(size_type sz);
        void copy_before_write();
    public:
        explicit basic_string(const Alloc& alloc = Alloc())
            :_ref_count(new long(1)), _alloc(alloc) {
                _start = _alloc.allocate(1);
                _finish = _start;
                *_finish = 0;
                _end_of_storage = _start;
            }

        basic_string(size_type count, CharT ch, const Alloc& alloc = Alloc())
            :_ref_count(new long(1)), _alloc(alloc) {
                initialize_aux(count, ch, HxSTL::true_type());
            }

        basic_string(const basic_string& other, size_type pos, size_type count = npos, 
            const Alloc& alloc = Alloc())
            :_ref_count(new long(1)), _alloc(alloc) {
                if (count == npos) count = other.size() - pos;
                initialize_aux(other._start + pos, count);
            }

        basic_string(const CharT* s, size_type count, const Alloc& alloc = Alloc())
            :_ref_count(new long(1)), _alloc(alloc) {
                initialize_aux(s, count);
            }

        basic_string(const CharT* s, const Alloc& alloc = Alloc())
            :_ref_count(new long(1)), _alloc(alloc) {
                const CharT* last = s;
                while (*last) ++last;
                initialize_aux(s, last - s);
            }

        template <class InputIt>
        basic_string(InputIt first, InputIt last, const Alloc& alloc = Alloc())
            :_ref_count(new long(1)), _alloc(alloc) {
                initialize_aux(first, last, HxSTL::is_integeral<InputIt>::type());
            }

        basic_string(const basic_string& other)
            :_ref_count(other._ref_count), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(other._alloc) {
                ++*_ref_count;
            }

        basic_string(const basic_string& other, const Alloc& alloc)
            :_ref_count(other._ref_count), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(alloc) {
                ++*_ref_count;
            }

        basic_string(basic_string&& other)
            :_ref_count(other._ref_count), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(HxSTL::move(other._alloc)) {
                other._ref_count = nullptr;
                other._start = other._finish = other._end_of_storage = nullptr; 
            }

        basic_string(basic_string&& other, const Alloc& alloc)
            :_ref_count(other._ref_count), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(alloc) {
                other._ref_count = nullptr;
                other._start = other._finish = other._end_of_storage = nullptr; 
            }

        ~basic_string() {
            if (_ref_count && (--*_ref_count == 0)) {
                HxSTL::destroy(_start, _finish);
                _alloc.deallocate(_start, capacity());
                delete _ref_count;
            }
        }

        basic_string& operator=(const basic_string& str) {
            return assign(str);
        }

        basic_string& operator=(basic_string&& str) {
            return assign(HxSTL::move(str));
        }

        basic_string& operator=(const CharT* s) {
            return assign(s);
        }

        basic_string& operator=(CharT ch) {
            return assign(1, ch);
        }
    public:
        basic_string& assign(size_type count, CharT ch) {
            assign_aux(count, ch, HxSTL::false_type());
            return *this;
        }

        basic_string& assign(const basic_string& str) {
            basic_string(str).swap(*this);
            return *this;
        }

        basic_string& assign(basic_string&& str) {
            basic_string(HxSTL::move(str)).swap(*this);
            return *this;
        }

        basic_string& assign(const CharT* s, size_type count) {
            assign_aux(s, count);
            return *this;
        }

        basic_string& assign(const CharT* s) {
            const CharT* last = s;
            while (*last) ++last;
            assign_aux(s, last - s);
            return *this;
        }

        template <class InputIt>
        basic_string& assign(InputIt first, InputIt last) {
            assign_aux(first, last, HxSTL::is_integeral<InputIt>::type());
            return *this;
        }

        allocator_type get_allocator() const { return _alloc; }

        reference at(size_type pos) {
            copy_before_write();
            return _start[pos];
        }

        const_reference at(size_type pos) const { return _start[pos]; }

        reference operator[](size_type pos) {
            copy_before_write();
            return _start[pos];
        }

        const_reference operator[](size_type pos) const { return _start[pos]; }
        
        reference front() {
            copy_before_write();
            return operator[](0);
        }

        const CharT& front() const { return operator[](0); }

        reference back() {
            copy_before_write();
            return operator[](size() - 1);
        }

        const CharT& back() const { return operator[](size() - 1); }
        
        const CharT* data() const { return _start; }

        const CharT* c_str() const { return _start; }

        iterator begin() {
            copy_before_write();
            return _start;
        }

        const_iterator begin() const { return _start; }

        const_iterator cbegin() const { return _start; }

        iterator end() {
            copy_before_write();
            return _finish;
        }

        const_iterator end() const { return _finish; }

        const_iterator cend() const { return _finish; }

//      reverse_iterator rbegin();
//
//      const_reverse_iterator rbegin() const;
//
//      const_reverse_iterator rcbegin();
//
//      reverse_iterator rend();
//
//      const_reverse_iterator rend() const;
//
//      const_reverse_iterator rcend();

        bool empty() const { begin() == end(); }

        size_type size() const { return end() - begin(); }

        size_type length() const { return size(); }

        size_type max_size() const { return Alloc::max_size(); }

        void reserve(size_type new_cap = 0);

        size_type capacity() const { return _end_of_storage - begin(); }

        void shrink_to_fit();

        void clear();

        basic_string& insert(size_type index, size_type count, CharT ch);

        basic_string& insert(size_type index, const CharT* s);

        basic_string& insert(size_type index, const CharT* s, size_type count);

        basic_string& insert(size_type index, const basic_string& str);

        basic_string& insert(size_type index, const basic_string& str, size_type index_str, size_type count = npos);

        iterator insert(const_iterator pos, CharT ch);

        iterator insert(const_iterator pos, size_type count, CharT ch);

        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last);

        basic_string& erase(size_type index = 0, size_type count = npos);

        iterator erase(const_iterator position);

        iterator erase(const_iterator first, const_iterator last);

        void push_back(CharT ch);

        void pop_back();

        basic_string& append(size_type count, CharT ch);

        basic_string& append(const basic_string& str);

        basic_string& append(const basic_string& str, size_type pos, size_type count = npos);

        basic_string& append(const CharT* s, size_type count);

        basic_string& append(const CharT* s);

        template <class InputIt>
        basic_string& append(InputIt first, InputIt last);

        basic_string& operator+=(const basic_string& str);

        basic_string& operator+=(CharT ch);

        basic_string& operator+=(const CharT* s);

        int compare(const basic_string& str) const;

        int compare(size_type pos, size_type count, const basic_string& str) const;

        int compare(size_type pos1, size_type count1, const basic_string& str, size_type pos2, size_type count2) const;

        int compare(const CharT* s) const;

        int compare(size_type pos, size_type count, const CharT* s) const;

        int compare(size_type pos, size_type count, const CharT* s, size_type pos2, size_type count2) const;
    };

    template <class CharT, class Alloc>
    template <class InputIt>
    void basic_string<CharT, Alloc>::initialize_aux(InputIt first, InputIt last, false_type) {
        size_type sz = HxSTL::distance(first, last);
        if (sz < DEFAULT_SIZE) sz = DEFAULT_SIZE;
        _start = _alloc.allocate(sz + 1);
        _finish = HxSTL::uninitialized_copy(first, last, _start);
        *_finish = 0;
        _end_of_storage = _start + sz;
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::initialize_aux(const_iterator first, size_type count) {
        size_type sz = count > DEFAULT_SIZE ? count : DEFAULT_SIZE;
        _start = _alloc.allocate(sz + 1);
        _finish = HxSTL::uninitialized_copy_n(first, count, _start);
        *_finish = 0;
        _end_of_storage = _start + sz;
    }
 
    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::initialize_aux(size_type count, CharT ch, true_type) {
        size_type sz = count > DEFAULT_SIZE ? count : DEFAULT_SIZE; 
        _start = _alloc.allocate(sz + 1);
        _finish = HxSTL::uninitialized_fill_n(_start, count, ch);
        *_finish = 0;
        _end_of_storage = _start + sz;
    }

    template <class CharT, class Alloc>
    template <class InputIt>
    void basic_string<CharT, Alloc>::assign_aux(InputIt first, InputIt last, false_type) {
        size_type count = HxSTL::distance(first, last);
        if (count <= size()) {
            // 拷贝并删除多余元素
            iterator old_finish = _finish;
            _finish = HxSTL::copy(first, last, _start);
            HxSTL::destroy(_finish, old_finish);
        } else if (count <= capacity()) {
            // 分开拷贝已初始化和未初始化区域
            size_type sz = size();
            HxSTL::copy_n(first, sz, _start);
            HxSTL::advance(first, sz);
            _finish = HxSTL::uninitialized_copy(first, last, _finish);
        } else {
            // Todo 处理异常
            _start = _alloc.allocate(count + 1);
            _finish = HxSTL::uninitialized_copy(first, last, _start);
            _end_of_storage = _finish;
        }
        *_finish = 0;
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::assign_aux(size_type count, CharT ch, true_type) {
        if (count <= size()) {
            // 填充并删除多余元素
            iterator old_finish = _finish;
            _finish = HxSTL::fill_n(_start, count, ch);
            HxSTL::destroy(_finish, old_finish);
        } else if (count <= capacity()) {
            // 分开填充已初始化和未初始化区域
            size_type sz = size();
            HxSTL::fill_n(_start, sz, ch);
            _finish = HxSTL::uninitialized_fill_n(_finish, count - sz, ch);
        } else {
            // Todo 处理异常
            _start = _alloc.allocate(count + 1);
            _finish = HxSTL::uninitialized_fill_n(_start, count, ch);
            _end_of_storage = _finish;
        }
        *_finish = 0;
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::assign_aux(const_iterator first, size_type count) {
        if (count <= size()) {
            // 拷贝并删除多余元素
            iterator old_finish = _finish;
            _finish = HxSTL::copy_n(first, count, _start);
            HxSTL::destroy(_finish, old_finish);
        } else if (count <= capacity()) {
            // 分开拷贝已初始化和未初始化区域
            size_type sz = size();
            HxSTL::copy_n(first, sz, _start);
            HxSTL::advance(first, sz);
            _finish = HxSTL::uninitialized_copy_n(first, count - sz, _finish);
        } else {
            // Todo 处理异常
            _start = _alloc.allocate(count + 1);
            _finish = HxSTL::uninitialized_copy_n(first, count, _start);
            _end_of_storage = _finish;
        }
        *_finish = 0;
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::insert_char_string_aux(size_type index, iterator first, iterator last) {
        size_type count = last - first;
        iterator old_finish = _finish;
        if (count > _end_of_storage - _finish) {
            size_type sz = count + size() > 2 * capacity() ? count + size() : 2 * capacity();
            iterator new_start = _alloc.allocate(sz + 1);
            _finish = uninitialized_move(_start, _start + index, new_start);
            _finish = uninitialized_copy_n(first, count, _finish);
            _finish = uninitialized_move(_start + index, old_finish, _finish);
            _start = new_start;
            _end_of_storage = new_start + sz;
        } else {
            size_type element_after = size() - index;

            if (count > element_after) {
                _finish = uninitialized_copy(first + element_after, last, _finish);
                _finish = uninitialized_copy(_start + index, old_finish, _finish);
                HxSTL::copy_n(first, element_after, _start + index);
            } else {
                _finish = uninitialized_move(_finish - count, _finish, _finish);
                HxSTL::copy_backward(_start + index, old_finish - count, old_finish);
                HxSTL::copy(first, last, _start + index);
            }
        }
        *_finish = 0;
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::reallocate_and_copy(size_type sz) {
        iterator new_start = _alloc.allocate(sz + 1);
        _finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
        *_finish = 0;
        _end_of_storage = new_start + sz;
        _start = new_start;
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::copy_before_write() {
        if (*_ref_count != 1) {
            reallocate_and_copy(capacity());
            --*_ref_count;
            _ref_count = new long(1);
        }
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::reserve(size_type new_cap) {
        copy_before_write();
        if (new_cap > capacity()) {
            copy_before_write();
            reallocate_and_copy(new_cap);
        } else if (new_cap < size()) {
            size_type del = size() - new_cap;
            HxSTL::__destroy(_finish - del + 1, _finish + 1);
            _finish = _finish - del;
            *_finish = 0;
            shrink_to_fit();
        } else {
            size_type unuse = capacity() - new_cap;
            _end_of_storage = _start + new_cap;
            _alloc.deallocate(_end_of_storage + 1, unuse);
        }
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::shrink_to_fit() {
        copy_before_write();
        int unuse = _end_of_storage - _finish;
        if (unuse) {
            _alloc.deallocate(_finish + 1, unuse);
            _end_of_storage = _finish;
        }
    }

    template <class CharT, class Alloc>
    basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::insert(size_type index, size_type count, CharT ch) {
        iterator old_finish = _finish;
        if (count > _end_of_storage - _finish) {
            size_type sz = count + size() > 2 * capacity() ? count + size() : 2 * capacity();
            iterator new_start = _alloc.allocate(sz + 1);
            _finish = uninitialized_move(_start, _start + index, new_start);
            _finish = uninitialized_fill_n(_finish, count, ch);
            _finish = uninitialized_move(_start + index, old_finish, _finish);
            *_finish = 0;
            _start = new_start;
            _end_of_storage = new_start + sz;
        } else {
            size_type element_after = size() - index;

            if (count > element_after) {
                _finish = uninitialized_fill_n(_finish, count - element_after, ch);
                _finish = uninitialized_move(_start + index, old_finish, _finish);
                *_finish = 0;
                HxSTL::fill_n(_start + index, element_after, ch);
            } else {
                _finish = uninitialized_move(_finish - count, _finish, _finish);
                *_finish = 0;
                HxSTL::copy_backward(_start + index, old_finish - count, old_finish);
                HxSTL::fill_n(_start + index, count, ch);
            }
        }
        return *this;
    }

    template <class CharT, class Alloc>
    basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::insert(size_type index, const CharT* s) {
        CharT *t = s;
        while (*t) ++t;
        insert_char_string_aux(index, s, t);
        return *this;
    }

    template <class CharT, class Alloc>
    basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::insert(size_type index, const CharT* s, size_type count) {
        insert_char_string_aux(index, s, s + count);
        return *this;
    }

    template <class CharT, class Alloc>
    basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::insert(size_type index, const basic_string<CharT, Alloc>& str) {
        insert_char_string_aux(index, str.cbegin(), str.cend());
        return *this;
    }

    template <class CharT, class Alloc>
    basic_string<CharT, Alloc>& basic_string<CharT, Alloc>::insert(size_type index, const basic_string<CharT, Alloc>& str, 
            size_type index_str, size_type count) {
        if (count == npos) {
            insert_char_string_aux(index, str.cbegin() + index_str, str.cend());
        } else {
            insert_char_string_aux(index, str.cbegin() + index_str, str.cbegin() + index_str + count);
        }
        return *this;
    }

    template <class CharT, class Alloc>
    typename basic_string<CharT, Alloc>::iterator
    basic_string<CharT, Alloc>::insert(const_iterator pos, CharT ch) {
    }

    template <class CharT, class Alloc>
    typename basic_string<CharT, Alloc>::iterator
    basic_string<CharT, Alloc>::insert(const_iterator pos, size_type count, CharT ch) {
    }

    template <class CharT, class Alloc>
    template <class InputIt>
    typename basic_string<CharT, Alloc>::iterator
    basic_string<CharT, Alloc>::insert(const_iterator pos, InputIt first, InputIt last) {
    }

}

#endif
