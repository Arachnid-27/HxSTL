#ifndef _BASIC_STRING_
#define _BASIC_STRING_


#include "allocator.h"
#include "uninitialized.h"
#include "stdexcept.h"


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
        template <class InputIt>
        iterator insert_aux(iterator pos, InputIt first, InputIt last, false_type);
        iterator insert_aux(iterator pos, size_type count, CharT ch, true_type);
        iterator erase_aux(iterator first, iterator last);
        template <class InputIt>
        void append_aux(InputIt first, InputIt last, false_type);
        void append_aux(size_type count, CharT ch, true_type);
        int compare_aux(const_iterator first1, const_iterator last1, const_iterator first2, const_iterator last2) const;
        template <class InputIt>
        void replace_aux(iterator first1, iterator last1, InputIt first2, InputIt last2, false_type);
        void replace_aux(iterator first, iterator last, size_type count, CharT ch, true_type);
        void resize_aux(size_type count, CharT ch);
        void destroy_and_reset(iterator new_start, iterator new_finish, iterator new_end_of_storage);
        iterator REMOVE_CONST(const_iterator it) { return _start + (it - _start); }
        const_iterator C_STR_END(const CharT* s) { while(*s) ++s; return s; }
    public:
        explicit basic_string(const Alloc& alloc = Alloc()): _alloc(alloc) {
                initialize_aux(0, 0, HxSTL::true_type());
            }

        basic_string(size_type count, CharT ch, const Alloc& alloc = Alloc()): _alloc(alloc) {
                initialize_aux(count, ch, HxSTL::true_type());
            }

        basic_string(const basic_string& other, size_type pos, size_type count = npos, 
            const Alloc& alloc = Alloc()): _alloc(alloc) {
                if (count == npos) count = other.size() - pos;
                initialize_aux(other.begin() + pos, count);
            }

        basic_string(const CharT* s, size_type count, const Alloc& alloc = Alloc()): _alloc(alloc) {
                initialize_aux(s, count);
            }

        basic_string(const CharT* s, const Alloc& alloc = Alloc()): _alloc(alloc) {
                initialize_aux(s, C_STR_END(s), HxSTL::false_type());
            }

        template <class InputIt>
        basic_string(InputIt first, InputIt last, const Alloc& alloc = Alloc()): _alloc(alloc) {
                initialize_aux(first, last, typename HxSTL::is_integeral<InputIt>::type());
            }

        basic_string(const basic_string& other): _alloc(other._alloc) {
                initialize_aux(other.begin(), other.end(), typename HxSTL::false_type());
            }

        basic_string(const basic_string& other, const Alloc& alloc):  _alloc(alloc) {
                initialize_aux(other.begin(), other.end(), typename HxSTL::false_type());
            }

        basic_string(basic_string&& other): _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(HxSTL::move(other._alloc)) {
                other._start = other._finish = other._end_of_storage = nullptr; 
            }

        basic_string(basic_string&& other, const Alloc& alloc): _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(alloc) {
                other._start = other._finish = other._end_of_storage = nullptr; 
            }

        ~basic_string() {
            if (_start) {
                HxSTL::destroy(_start, _finish);
                _alloc.deallocate(_start, capacity());
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
            assign_aux(count, ch, HxSTL::true_type());
            return *this;
        }

        basic_string& assign(const basic_string& str) {
            basic_string(str).swap(*this);
            return *this;
        }

        basic_string& assign(const basic_string& str, size_type pos, size_type count = npos) {
            if (count == npos) count = str.size() - pos;
            const_iterator first = str.cbegin() + pos;
            assign_aux(first, first + count, HxSTL::false_type());
            return *this;
        }

        basic_string& assign(basic_string&& str) {
            basic_string(HxSTL::move(str)).swap(*this);
            return *this;
        }

        basic_string& assign(const CharT* s, size_type count) {
            assign_aux(s, s + count, HxSTL::false_type());
            return *this;
        }

        basic_string& assign(const CharT* s) {
            assign_aux(s, C_STR_END(s), HxSTL::false_type());
            return *this;
        }

        template <class InputIt>
        basic_string& assign(InputIt first, InputIt last) {
            assign_aux(first, last, typename HxSTL::is_integeral<InputIt>::type());
            return *this;
        }

        allocator_type get_allocator() const { return _alloc; }

        reference at(size_type pos) {
            if (pos >= size()) throw HxSTL::out_of_range();
            return _start[pos];
        }

        const_reference at(size_type pos) const {
            if (pos >= size()) throw HxSTL::out_of_range();
            return _start[pos];
        }

        reference operator[](size_type pos) { return _start[pos]; }

        const_reference operator[](size_type pos) const { return _start[pos]; }
        
        reference front() { return operator[](0); }

        const CharT& front() const { return operator[](0); }

        reference back() { return operator[](size() - 1); }

        const CharT& back() const { return operator[](size() - 1); }
        
        const CharT* data() const { return begin(); }

        const CharT* c_str() const { return begin(); }

        iterator begin() { return _start; }

        const_iterator begin() const { return _start; }

        const_iterator cbegin() const { return _start; }

        iterator end() { return _finish; }

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

        bool empty() const { return begin() == end(); }

        size_type size() const { return end() - begin(); }

        size_type length() const { return size(); }

        size_type max_size() const { return _alloc.max_size(); }

        void reserve(size_type new_cap = 0);

        size_type capacity() const { return _end_of_storage - _start; }

        void shrink_to_fit() { reserve(); }

        void clear() { erase(begin(), end()); }

        basic_string& insert(size_type index, size_type count, CharT ch) {
            if (index > size()) throw HxSTL::out_of_range();
            if (size() + count > max_size()) throw HxSTL::length_error();
            insert_aux(_start + index, count, ch, HxSTL::true_type());
            return *this;
        }

        basic_string& insert(size_type index, const CharT* s) {
            if (index > size()) throw HxSTL::out_of_range();
            const CharT *last = C_STR_END(s);
            if (size() + last - s > max_size()) throw HxSTL::length_error();
            insert_aux(_start + index, s, last, HxSTL::false_type());
            return *this;
        }

        basic_string& insert(size_type index, const CharT* s, size_type count) {
            if (index > size()) throw HxSTL::out_of_range();
            if (size() + count > max_size()) throw HxSTL::length_error();
            insert_aux(_start + index, s, s + count, HxSTL::false_type());
            return *this;
        }

        basic_string& insert(size_type index, const basic_string& str) {
            if (index > size()) throw HxSTL::out_of_range();
            if (size() + str.size() > max_size()) throw HxSTL::length_error();
            insert_aux(_start + index, str.cbegin(), str.cend(), HxSTL::false_type());
            return *this;
        }

        basic_string& insert(size_type index, const basic_string& str, size_type index_str, size_type count = npos) {
            if (index > size() || index_str > str.size()) throw HxSTL::out_of_range();
            if (size() + count > max_size()) throw HxSTL::length_error();
            if (count == npos) count = str.size() - index_str;
            const_iterator first = str.cbegin() + index_str;
            insert_aux(_start + index, first, first + count, HxSTL::false_type()); 
            return *this;
        }

        iterator insert(const_iterator pos, CharT ch) {
            return insert_aux(REMOVE_CONST(pos), 1, ch, HxSTL::true_type());
        }

        iterator insert(const_iterator pos, size_type count, CharT ch) {
            return insert_aux(REMOVE_CONST(pos), count, ch, HxSTL::true_type());
        }

        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            return insert_aux(REMOVE_CONST(pos), first, last, typename HxSTL::is_integeral<InputIt>::type());
        }

        basic_string& erase(size_type index = 0, size_type count = npos) {
            if (count == npos) count = size() - index;
            erase_aux(_start + index, _start + index + count);
            return *this;
        }

        iterator erase(const_iterator position) {
            return erase_aux(REMOVE_CONST(position), REMOVE_CONST(position) + 1);
        }

        iterator erase(const_iterator first, const_iterator last) {
            return erase_aux(REMOVE_CONST(first), REMOVE_CONST(last));
        }

        void push_back(CharT ch) { append(1, ch); }

        void pop_back() { erase(size() - 1, 1); }

        basic_string& append(size_type count, CharT ch) {
            append_aux(count, ch, HxSTL::true_type());
            return *this;
        }

        basic_string& append(const basic_string& str) {
            append_aux(str.cbegin(), str.cend(), HxSTL::false_type());
            return *this;
        }

        basic_string& append(const basic_string& str, size_type pos, size_type count = npos) {
            if (count == npos) count = str.size() - pos;
            const_iterator first = str.cbegin() + pos;
            append_aux(first, first + count, HxSTL::false_type());
            return *this;
        }

        basic_string& append(const CharT* s, size_type count) {
            append_aux(s, s + count, HxSTL::false_type());
            return *this;
        }

        basic_string& append(const CharT* s) {
            append_aux(s, C_STR_END(s), HxSTL::false_type());
            return *this;
        }

        template <class InputIt>
        basic_string& append(InputIt first, InputIt last) {
            append_aux(first, last, typename HxSTL::is_integeral<InputIt>::type());
            return *this;
        }

        basic_string& operator+=(const basic_string& str) {
            return append(str);
        }

        basic_string& operator+=(CharT ch) {
            return append(1, ch);
        }

        basic_string& operator+=(const CharT* s) {
            return append(s);
        }

        int compare(const basic_string& str) const {
            return compare_aux(cbegin(), cend(), str.cbegin(), str.cend());
        }

        int compare(size_type pos, size_type count, const basic_string& str) const {
            return compare_aux(cbegin() + pos, cbegin() + pos + count, str.cbegin(), str.cend());
        }

        int compare(size_type pos1, size_type count1, const basic_string& str, 
                size_type pos2, size_type count2) const {
            return compare_aux(cbegin() + pos1, cbegin() + pos1 + count1, 
                    str.cbegin() + pos2, str.cend() + pos2 + count2);
        }

        int compare(const CharT* s) const {
            return compare_aux(cbegin(), cend(), s, C_STR_END(s));
        }

        int compare(size_type pos, size_type count, const CharT* s) const {
            return compare_aux(cbegin() + pos, cbegin() + pos + count, s, C_STR_END(s));
        }

        int compare(size_type pos, size_type count, const CharT* s, size_type pos2, size_type count2) const {
            return compare_aux(cbegin() + pos, cbegin() + pos + count, s + pos2, s + pos2 + count2);
        }

        basic_string& replace(size_type pos, size_type count, const basic_string& str) {
            replace_aux(pos, pos + count, str.cbegin(), str.cend(), HxSTL::false_type());
            return *this;
        }

        basic_string& replace(const_iterator first, const_iterator last, const basic_string& str) {
            replace_aux(REMOVE_CONST(first), REMOVE_CONST(last), str.cbegin(), str.cend(), HxSTL::false_type());
            return *this;
        }

        basic_string& replace(size_type pos, size_type count, const basic_string& str, size_type pos2, size_type count2 = npos) {
            if (count2 == npos) count2 = str.size() - pos2;
            replace_aux(pos, pos + count, str.cbegin(), str.cbegin() + pos2, str.cbegin() + pos2 + count2, HxSTL::false_type());
            return *this;
        }

        template <class InputIt>
        basic_string& replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2) {
            replace_aux(REMOVE_CONST(first), REMOVE_CONST(last), first2, last2, typename HxSTL::is_integeral<InputIt>::type());
            return *this;
        }

        basic_string& replace(size_type pos, size_type count, const CharT* cstr, size_type count2) {
            replace_aux(pos, pos + count, cstr, cstr + count2, HxSTL::false_type());
            return *this;
        }

        basic_string& replace(const_iterator first, const_iterator last, const CharT* cstr, size_type count2) {
            replace_aux(REMOVE_CONST(first), REMOVE_CONST(last), cstr, cstr + count2, HxSTL::false_type());
            return *this;
        }

        basic_string& replace(size_type pos, size_type count, const CharT* cstr) {
            replace_aux(pos, pos + count, cstr, C_STR_END(cstr), HxSTL::false_type());
            return *this;
        }

        basic_string& replace(const_iterator first, const_iterator last, const CharT* cstr) {
            replace_aux(REMOVE_CONST(first), REMOVE_CONST(last), cstr, C_STR_END(cstr), HxSTL::false_type());
            return *this;
        }

        basic_string& replace(size_type pos, size_type count, size_type count2, CharT ch) {
            replace_aux(pos, pos + count, count2, ch, HxSTL::true_type());
            return *this;
        }

        basic_string& replace(const_iterator first, const_iterator last, size_type count2, CharT ch) {
            replace_aux(REMOVE_CONST(first), REMOVE_CONST(last), count2, ch, HxSTL::true_type());
            return *this;
        }

        basic_string substr(size_type pos = 0, size_type count = npos) const {
            return basic_string(*this, pos, count);
        }

        size_type copy(CharT* dest, size_type count, size_type pos = 0) const;

        void resize(size_type count) {
            resize_aux(count, CharT());
        }

        void resize(size_type count, CharT ch) {
            resize_aux(count, ch);
        }

        void swap(basic_string& other) {
            HxSTL::swap(_start, other._start);
            HxSTL::swap(_finish, other._finish);
            HxSTL::swap(_end_of_storage, other._end_of_storage);
        }

        size_type find(const basic_string& str, size_type pos = 0) const;

        size_type find(const CharT* s, size_type pos, size_type count) const;

        size_type find(const CharT* s, size_type pos = 0) const;

        size_type find(CharT ch, size_type pos = 0) const;

        size_type rfind(const basic_string& str, size_type pos = npos) const;

        size_type rfind(const CharT* s, size_type pos, size_type count) const;

        size_type rfind(const CharT* s, size_type pos = npos) const;

        size_type rfind(CharT ch, size_type pos = npos) const;

        size_type find_first_of(const basic_string& str, size_type pos = 0) const;

        size_type find_first_of(const CharT* s, size_type pos, size_type count) const;

        size_type find_first_of(const CharT* s, size_type pos = 0) const;

        size_type find_first_of(CharT ch, size_type pos = 0) const;

        size_type find_first_not_of(const basic_string& str, size_type pos = 0) const;

        size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const;

        size_type find_first_not_of(const CharT* s, size_type pos = 0) const;

        size_type find_first_not_of(CharT ch, size_type pos = 0) const;

        size_type find_last_of(const basic_string& str, size_type pos = npos) const;

        size_type find_last_of(const CharT* s, size_type pos, size_type count) const;

        size_type find_last_of(const CharT* s, size_type pos = npos) const;

        size_type find_last_of(CharT ch, size_type pos = npos) const;

        size_type find_last_not_of(const basic_string& str, size_type pos = npos) const;

        size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const;

        size_type find_last_not_of(const CharT* s, size_type pos = npos) const;

        size_type find_last_not_of(CharT ch, size_type pos = npos) const;
    };

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::destroy_and_reset(iterator new_start, 
            iterator new_finish, iterator new_end_of_storage) {
        if (_start) {
            HxSTL::destroy(_start, _finish);
            _alloc.deallocate(_start, capacity());
        }
        _start = new_start;
        _finish = new_finish;
        _end_of_storage = new_end_of_storage;
        *_finish = 0;
    }

    template <class CharT, class Alloc>
    template <class InputIt>
    void basic_string<CharT, Alloc>::initialize_aux(InputIt first, InputIt last, false_type) {
        size_type count = HxSTL::distance(first, last);
        if (count < DEFAULT_SIZE) count = DEFAULT_SIZE;
        _start = _alloc.allocate(count + 1);
        _finish = HxSTL::uninitialized_copy(first, last, _start);
        *_finish = 0;
        _end_of_storage = _start + count;
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
        size_type cap = capacity();
        if (count > cap) {
            // 保留空间不足
            size_type new_sz = count > 2 * cap ? count : 2 * cap;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_copy(first, last, new_start);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else if (count <= size()) {
            // 拷贝并删除多余元素
            iterator old_finish = _finish;
            _finish = HxSTL::copy(first, last, _start);
            HxSTL::destroy(_finish, old_finish);
            *_finish = 0;
        } else {
            // 分开拷贝已初始化和未初始化区域
            size_type sz = size();
            HxSTL::copy_n(first, sz, _start);
            HxSTL::advance(first, sz);
            _finish = HxSTL::uninitialized_copy(first, last, _finish);
            *_finish = 0;
        }
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::assign_aux(size_type count, CharT ch, true_type) {
        size_type cap = capacity();
        if (count > cap) {
            // 保留空间不足
            size_type new_sz = count > 2 * cap ? count : 2 * cap;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_fill_n(new_start, count, ch);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else if (count <= size()) {
            // 填充并删除多余元素
            iterator old_finish = _finish;
            _finish = HxSTL::fill_n(_start, count, ch);
            HxSTL::destroy(_finish, old_finish);
            *_finish = 0;
        } else {
            // 分开填充已初始化和未初始化区域
            size_type sz = size();
            HxSTL::fill_n(_start, sz, ch);
            _finish = HxSTL::uninitialized_fill_n(_finish, count - sz, ch);
            *_finish = 0;
        }
    }

    template <class CharT, class Alloc>
    template <class InputIt>
    typename basic_string<CharT, Alloc>::iterator
    basic_string<CharT, Alloc>::insert_aux(iterator pos, InputIt first, InputIt last, false_type) {
        if (first == last) return pos;

        size_type index = pos - _start;
        size_type count = HxSTL::distance(first, last);
        size_type cap = capacity();
        size_type sz = size();
        if (count > cap - sz) {
            // 保留空间不足
            size_type new_sz = 2 * cap > sz + count ? 2 * cap : sz + count;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, pos, new_start);
            new_finish = HxSTL::uninitialized_copy(first, last, new_finish);
            new_finish = HxSTL::uninitialized_copy(pos, _finish, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else {
            size_type element_after = _finish - pos;
            iterator old_finish = _finish;
            if (count > element_after) {
                // 插入点之后元素少于待插入元素
                _finish = HxSTL::uninitialized_copy(pos, _finish, _finish + (count - element_after));
                HxSTL::copy_n(first, element_after, pos);
                HxSTL::advance(first, element_after);
                HxSTL::uninitialized_copy(first, last, old_finish);
            } else {
                // 插入点之后元素不少于待插入元素
                _finish = HxSTL::uninitialized_copy(_finish - count, _finish, _finish);
                HxSTL::copy_backward(pos, old_finish - count, old_finish);
                HxSTL::copy(first, last, pos);
            }
            *_finish = 0;
        }
        return _start + index;
    }

    template <class CharT, class Alloc>
    typename basic_string<CharT, Alloc>::iterator
    basic_string<CharT, Alloc>::insert_aux(iterator pos, size_type count, CharT ch, true_type) {
        if (count == 0) return pos;

        size_type index = pos - _start;
        size_type cap = capacity();
        size_type sz = size();
        if (count > cap - sz) {
            // 保留空间不足
            size_type new_sz = 2 * cap > sz + count ? 2 * cap : sz + count;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, pos, new_start);
            new_finish = HxSTL::uninitialized_fill_n(new_finish, count, ch);
            new_finish = HxSTL::uninitialized_copy(pos, _finish, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else {
            size_type element_after = _finish - pos;
            iterator old_finish = _finish;
            if (count > element_after) {
                // 插入点之后元素少于待插入元素
                _finish = HxSTL::uninitialized_copy(pos, _finish, _finish + (count - element_after));
                HxSTL::fill_n(pos, count, ch);
                HxSTL::uninitialized_fill_n(old_finish, count - element_after, ch);
            } else {
                // 插入点之后元素不少于待插入元素
                _finish = HxSTL::uninitialized_copy(_finish - count, _finish, _finish);
                HxSTL::copy_backward(pos, old_finish - count, old_finish);
                HxSTL::fill_n(pos, count, ch);
            }
            *_finish = 0;
        }
        return _start + index;
    }

    template <class CharT, class Alloc>
    typename basic_string<CharT, Alloc>::iterator
    basic_string<CharT, Alloc>::erase_aux(iterator first, iterator last) {
        iterator old_finish = _finish;
        _finish = HxSTL::copy(last, _finish, first);
        HxSTL::destroy(_finish, old_finish);
        return first;
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::append_aux(size_type count, CharT ch, true_type) {
        size_type cap = capacity();
        size_type sz = size();
        if (count > cap - sz) {
            // 保留空间不足
            size_type new_sz = 2 * cap > sz + count ? 2 * cap : sz + count;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            new_finish = HxSTL::uninitialized_fill_n(new_finish, count, ch);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else {
            // 直接填充到末尾
            _finish = HxSTL::uninitialized_fill_n(_finish, count, ch);
            *_finish = 0;
        }
    }

    template <class CharT, class Alloc>
    template <class InputIt>
    void basic_string<CharT, Alloc>::append_aux(InputIt first, InputIt last, false_type) {
        size_type count = HxSTL::distance(first, last);
        size_type cap = capacity();
        size_type sz = size();
        if (count > cap - sz) {
            // 保留空间不足
            size_type new_sz = 2 * cap > sz + count ? 2 * cap : sz + count;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            new_finish = HxSTL::uninitialized_copy(first, last, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else {
            // 直接复制到末尾
            _finish = HxSTL::uninitialized_copy(first, last, _finish);
            *_finish = 0;
        }
    }

    template <class CharT, class Alloc>
    int basic_string<CharT, Alloc>::compare_aux(const_iterator first1, const_iterator last1, 
            const_iterator first2, const_iterator last2) const {
        while (first1 != last1 && first2 != last2) {
            if (*first1 != *first2) {
                return *first1 < *first2 ? -1 : 1;
            }
            ++first1;
            ++first2;
        }
        return first1 == last1 ? first2 - last2 : last1 - first1;
    }

    template <class CharT, class Alloc>
    template <class InputIt>
    void basic_string<CharT, Alloc>::replace_aux(iterator first1, iterator last1, 
            InputIt first2, InputIt last2, false_type) {
        size_type count1 = last1 - first1;
        size_type count2 = HxSTL::distance(first2, last2);

        if (count2 <= count1) {
            // 替换元素不大于待替换元素
            HxSTL::copy(first2, last2, first1);
            _finish = HxSTL::copy(last1, _finish, first1 + count2);
            *_finish = 0;
        } else {
            size_type count = size() - count1 + count2;
            size_type cap = capacity();
            if (count > cap) {
                // 保留空间不足
                size_type new_sz = 2 * cap > count ? 2 * cap : count;
                iterator new_start = _alloc.allocate(new_sz + 1);
                iterator new_finish = HxSTL::uninitialized_copy(_start, first1, new_start);
                new_finish = HxSTL::uninitialized_copy(first2, last2, new_finish);
                new_finish = HxSTL::uninitialized_copy(last1, _finish, new_finish);
                destroy_and_reset(new_start, new_finish, new_start + new_sz);
            } else {
                size_type element_after = _finish - first1;
                iterator old_finish = _finish;
                if (count2 > element_after) {
                    // 插入点之后元素少于待插入元素
                    _finish = HxSTL::uninitialized_copy(last1, _finish, _finish + (count2 - element_after));
                    HxSTL::copy_n(first2, element_after, first1);
                    HxSTL::advance(first2, element_after);
                    HxSTL::uninitialized_copy(first2, last2, old_finish);
                } else {
                    // 插入点之后元素不少于待插入元素
                    _finish = HxSTL::uninitialized_copy(_finish - (count2 - count1), _finish, _finish);
                    HxSTL::copy_backward(last1, old_finish - (count2 - count1), old_finish);
                    HxSTL::copy(first2, last2, first1);
                }
                *_finish = 0;
            }
        }
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::replace_aux(iterator first, iterator last, size_type count, CharT ch, true_type) {
        if (count <= last - first) {
            // 替换元素不大于待替换元素
            HxSTL::fill_n(first, count, ch);
            _finish = HxSTL::copy(last, _finish, first + count);
            *_finish = 0;
        } else {
            size_type count = size() - (last - first) + count;
            size_type cap = capacity();
            if (count > cap) {
                // 保留空间不足
                size_type new_sz = 2 * cap > count ? 2 * cap : count;
                if (new_sz < DEFAULT_SIZE) new_sz = DEFAULT_SIZE;
                iterator new_start = _alloc.allocate(new_sz + 1);
                iterator new_finish = HxSTL::uninitialized_copy(_start, first, new_start);
                new_finish = HxSTL::uninitialized_fill_n(new_finish, count, ch);
                new_finish = HxSTL::uninitialized_copy(last, _finish, new_finish);
                destroy_and_reset(new_start, new_finish, new_start + new_sz);
            } else {
                size_type element_after = _finish - first;
                iterator old_finish = _finish;
                if (count > element_after) {
                    // 插入点之后元素少于待插入元素
                    _finish = HxSTL::uninitialized_copy(last, _finish, _finish + (count - element_after));
                    HxSTL::fill(first, old_finish, ch);
                    HxSTL::uninitialized_fill_n(old_finish, count - element_after, ch);
                } else {
                    // 插入点之后元素不少于待插入元素
                    _finish = HxSTL::uninitialized_copy(_finish - (count - (last - first)), _finish, _finish);
                    HxSTL::copy_backward(last, old_finish - (count - (last - first)), old_finish);
                    HxSTL::fill_n(first, count, ch);
                }
                *_finish = 0;
            }
        }
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::resize_aux(size_type count, CharT ch) {
        if (count <= size()) {
            reserve();
        } else {
            if (count < DEFAULT_SIZE) count = DEFAULT_SIZE;
            iterator new_start = _alloc.allocate(count + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            new_finish = HxSTL::uninitialized_fill_n(new_finish, count - size(), ch);
            destroy_and_reset(new_start, new_finish, new_start + count);
        }
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::reserve(size_type new_cap) {
        if (new_cap > max_size()) throw HxSTL::length_error();

        size_type cap = capacity();
        if (new_cap > cap) {
            if (new_cap < 2 * cap) new_cap = 2 * cap;
        } else { 
            if (new_cap <= size()) new_cap = size();
            if (new_cap < DEFAULT_SIZE) new_cap = DEFAULT_SIZE;
        }

        iterator new_start = _alloc.allocate(new_cap + 1);
        iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
        destroy_and_reset(new_start, new_finish, new_start + new_cap);
    }

    template <class CharT, class Alloc>
    typename basic_string<CharT, Alloc>::size_type 
    basic_string<CharT, Alloc>::copy(CharT* dest, size_type count, size_type pos) const {
        if (size() - pos < count) {
            HxSTL::copy(_start + pos, _finish, dest);
            return size() - pos;
        } else {
            HxSTL::copy_n(_start, count, dest);
            return count;
        }
    }

    template <class CharT, class Alloc>
    bool operator==(const basic_string<CharT, Alloc>& lhs, const basic_string<CharT, Alloc>& rhs) {
        return lhs.compare(rhs) == 0;
    }

    template <class CharT, class Alloc>
    bool operator!=(const basic_string<CharT, Alloc>& lhs, const basic_string<CharT, Alloc>& rhs) {
        return !(lhs == rhs);
    }

    template <class CharT, class Alloc>
    bool operator< (const basic_string<CharT, Alloc>& lhs, const basic_string<CharT, Alloc>& rhs) {
        return lhs.compare(rhs) < 0;
    }

    template <class CharT, class Alloc>
    bool operator<=(const basic_string<CharT, Alloc>& lhs, const basic_string<CharT, Alloc>& rhs) {
        return !(rhs < lhs);
    }
    template <class CharT, class Alloc>
    bool operator> (const basic_string<CharT, Alloc>& lhs, const basic_string<CharT, Alloc>& rhs) {
        return rhs < lhs;
    }

    template <class CharT, class Alloc>
    bool operator>=(const basic_string<CharT, Alloc>& lhs, const basic_string<CharT, Alloc>& rhs) {
        return !(lhs < rhs);
    }

}


#endif
