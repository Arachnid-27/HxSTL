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
        long* _used;
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
        void insert_aux(iterator pos, size_type count, CharT ch, true_type);
        iterator erase_aux(iterator first, iterator last);
        template <class InputIt>
        void append_aux(InputIt first, InputIt last, false_type);
        void append_aux(size_type count, CharT ch, true_type);
        int compare_aux(const_iterator first1, const_iterator last1, const_iterator first2, const_iterator last2) const;
        void dec_ref();
        void copy_before_write();
        void destroy_and_reset(iterator new_start, iterator new_finish, iterator new_end_of_storage);
    public:
        explicit basic_string(const Alloc& alloc = Alloc())
            :_used(new long(1)), _alloc(alloc) {
                _start = _alloc.allocate(1);
                _finish = _start;
                *_finish = 0;
                _end_of_storage = _start;
            }

        basic_string(size_type count, CharT ch, const Alloc& alloc = Alloc())
            :_used(new long(1)), _alloc(alloc) {
                initialize_aux(count, ch, HxSTL::true_type());
            }

        basic_string(const basic_string& other, size_type pos, size_type count = npos, 
            const Alloc& alloc = Alloc())
            :_used(new long(1)), _alloc(alloc) {
                if (count == npos) count = other.size() - pos;
                initialize_aux(other._start + pos, count);
            }

        basic_string(const CharT* s, size_type count, const Alloc& alloc = Alloc())
            :_used(new long(1)), _alloc(alloc) {
                initialize_aux(s, count);
            }

        basic_string(const CharT* s, const Alloc& alloc = Alloc())
            :_used(new long(1)), _alloc(alloc) {
                const CharT* last = s;
                while (*last) ++last;
                initialize_aux(s, last - s);
            }

        template <class InputIt>
        basic_string(InputIt first, InputIt last, const Alloc& alloc = Alloc())
            :_used(new long(1)), _alloc(alloc) {
                initialize_aux(first, last, HxSTL::is_integeral<InputIt>::type());
            }

        basic_string(const basic_string& other)
            :_used(other._used), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(other._alloc) {
                ++*_used;
            }

        basic_string(const basic_string& other, const Alloc& alloc)
            :_used(other._used), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(alloc) {
                ++*_used;
            }

        basic_string(basic_string&& other)
            :_used(other._used), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(HxSTL::move(other._alloc)) {
                other._used = nullptr;
                other._start = other._finish = other._end_of_storage = nullptr; 
            }

        basic_string(basic_string&& other, const Alloc& alloc)
            :_used(other._used), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(alloc) {
                other._used = nullptr;
                other._start = other._finish = other._end_of_storage = nullptr; 
            }

        ~basic_string() {
            dec_ref();
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

        basic_string& assign(const basic_string& str, size_type pos, size_type count = npos) {
            if (count == npos) count = str.size() - pos;
            const_iterator first = str.cbegin() + pos;
            assign_aux(first, first + count, HxSTL::false_type());
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
            assign_aux(s, s + count, HxSTL::false_type());
            return *this;
        }

        basic_string& assign(const CharT* s) {
            const CharT* last = s;
            while (*last) ++last;
            assign_aux(s, last, HxSTL::false_type());
            return *this;
        }

        template <class InputIt>
        basic_string& assign(InputIt first, InputIt last) {
            assign_aux(first, last, HxSTL::is_integeral<InputIt>::type());
            return *this;
        }

        allocator_type get_allocator() const { return _alloc; }

        reference at(size_type pos) {
            if (pos >= size()) throw HxSTL::out_of_range();
            copy_before_write();
            return _start[pos];
        }

        const_reference at(size_type pos) const {
            if (pos >= size()) throw HxSTL::out_of_range();
            return _start[pos];
        }

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

        bool empty() const { cbegin() == cend(); }

        size_type size() const { return cend() - cbegin(); }

        size_type length() const { return size(); }

        size_type max_size() const { return Alloc::max_size(); }

        void reserve(size_type new_cap = 0);

        size_type capacity() const { return _end_of_storage - _start; }

        void shrink_to_fit() { reserve(); }

        void clear() { erase(_start, _finish); }

        basic_string& insert(size_type index, size_type count, CharT ch) {
            insert_aux(_start + index, count, ch, HxSTL::true_type());
            return *this;
        }

        basic_string& insert(size_type index, const CharT* s) {
            const CharT* last = s;
            while (*last) ++last;
            insert_aux(_start + index, s, last, HxSTL::false_type());
            return *this;
        }

        basic_string& insert(size_type index, const CharT* s, size_type count) {
            insert_aux(_start + index, s, s + count, HxSTL::false_type());
            return *this;
        }

        basic_string& insert(size_type index, const basic_string& str) {
            insert_aux(_start + index, str.cbegin(), str.cend(), HxSTL::false_type());
            return *this;
        }

        basic_string& insert(size_type index, const basic_string& str, size_type index_str, size_type count = npos) {
            if (count == npos) count = str.size() - index_str;
            const_iterator first = str.cbegin() + index_str;
            insert_aux(_start + index, first, first + count, HxSTL::false_type()); 
            return *this;
        }

        iterator insert(const_iterator pos, CharT ch) {
            return insert_aux(_start + (pos - cbegin()), 1, ch, HxSTL::true_type());
        }

        iterator insert(const_iterator pos, size_type count, CharT ch) {
            return insert_aux(_start + (pos - cbegin()), count, ch, HxSTL::true_type());
        }

        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            return insert(_start + (pos - cbegin()), first, last, HxSTL::is_integeral<InputIt>::type());
        }

        basic_string& erase(size_type index = 0, size_type count = npos) {
            if (count == npos) count = size() - index;
            erase_aux(_start + index, _start + index + count);
            return *this;
        }

        iterator erase(const_iterator position) {
            return erase_aux(_start + (position - _start), _start + (position - _start) + 1);
        }

        iterator erase(const_iterator first, const_iterator last) {
            return erase_aux(_start + (first - _start), _start + (last - _start));
        }

        void push_back(CharT ch) { append(ch); }

        void pop_back() { erase(_finish); }

        basic_string& append(size_type count, CharT ch) {
            append_aux(count, ch, HxSTL::true_type());
            return *this;
        }

        basic_string& append(const basic_string& str) {
            append_aux(str.cbegin(), str.cend(), HxSTL::false_type());
            return *this;
        }

        basic_string& append(const basic_string& str, size_type pos, size_type count = npos) {
            if (count == pos) count = str.size() - pos;
            const_iterator first = str.cbegin() + pos;
            append_aux(first, first + index, HxSTL::false_type());
            return *this;
        }

        basic_string& append(const CharT* s, size_type count) {
            append_aux(s, s + count, HxSTL::false_type());
            return *this;
        }

        basic_string& append(const CharT* s) {
            const CharT* last = s;
            while (*last) ++last;
            append_aux(s, last, HxSTL::false_type());
            return *this;
        }

        template <class InputIt>
        basic_string& append(InputIt first, InputIt last) {
            append_aux(first, last, HxSTL::is_integeral<InputIt>::type());
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
            const_iterator last = s;
            while (*last) ++last;
            return compare_aux(cbegin(), cend(), s, last);
        }

        int compare(size_type pos, size_type count, const CharT* s) const {
            const_iterator last = s;
            while (*last) ++last;
            return compare_aux(cbegin() + pos, cbegin() + pos + count, s, last);
        }

        int compare(size_type pos, size_type count, const CharT* s, size_type pos2, size_type count2) const {
            return compare_aux(cbegin() + pos, cbegin() + pos + count, s + pos2, s + pos2 + count2);
        }

        basic_string& replace(size_type pos, size_type count, const basic_string& str);

        basic_string& replace(const_iterator first, const_iterator last, const basic_string& str);

        basic_string& replace(size_type pos, size_type count, const basic_string& str, size_type pos2, size_type count2 = npos);

        template <class InputIt>
        basic_string& replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2);

        basic_string& replace(size_type pos, size_type count, const CharT* cstr, size_type count2);

        basic_string& replace(const_iterator first, const_iterator last, const CharT* cstr, size_type count2);

        basic_string& replace(size_type pos, size_type count, const CharT* cstr);

        basic_string& replace(const_iterator first, const_iterator last, const CharT* cstr);

        basic_string& replace(size_type pos, size_type count, size_type count2, CharT ch);

        basic_string& replace(const_iterator first, const_iterator last, size_type count2, CharT ch);

        basic_string substr(size_type pos = 0, size_type count = npos) const;

        size_type copy(CharT* dest, size_type count, size_type pos = 0) const;

        void resize(size_type count);

        void resize(size_type count, CharT ch);

        void swap(basic_string& other) {
            HxSTL::swap(_used, other._used);
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
    void basic_string<CharT, Alloc>::dec_ref() {
        if (_used && (--*_used == 0)) {
            HxSTL::destroy(_start, _finish + 1);
            _alloc.deallocate(_start, capacity() + 1);
            delete _used;
        }
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::copy_before_write() {
        if (*_used > 1) {
            iterator new_start = _alloc.allocate(capacity() + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            destroy_and_reset(new_start, new_finish, new_start + capacity());
        }
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::destroy_and_reset(iterator new_start, 
            iterator new_finish, iterator new_end_of_storage) {
        dec_ref();
        _used = new long(1);
        _start = new_start;
        _finish = new_finish;
        _end_of_storage = new_end_of_storage;
        *_finish = 0;
    }

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
        if (count > capacity()) {
            // 保留空间不足
            size_type new_sz = count > 2 * capacity() ? count : 2 * capacity();
            if (new_sz < DEFAULT_SIZE) new_sz = DEFAULT_SIZE;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_copy(first, last, new_start);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else if (*_used > 1) {
            // 写时拷贝
            iterator new_start = _alloc.allocate(capacity() + 1);
            iterator new_finish = HxSTL::uninitialized_copy(first, last, new_start);
            destroy_and_reset(new_start, new_finish, new_start + count);
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
        if (count > capacity()) {
            // 保留空间不足
            size_type new_sz = count > 2 * capacity() ? count : 2 * capacity();
            if (new_sz < DEFAULT_SIZE) new_sz = DEFAULT_SIZE;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_fill_n(new_start, count, ch);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else if (*_used > 1) {
            // 写时拷贝
            iterator new_start = _alloc.allocate(capacity() + 1);
            iterator new_finish = HxSTL::uninitialized_fill_n(new_start, count, ch);
            destroy_and_reset(new_start, new_finish, new_start + count);
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
        if (count < _end_of_storage - _finish) {
            // 保留空间不足
            size_type new_sz = 2 * capacity() > size() + count ? 2 * capacity() : size() + count;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, pos, new_start);
            new_finish = HxSTL::uninitialized_copy(first, last, new_finish);
            new_finish = HxSTL::uninitialized_copy(pos, _finish, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else if (*_used > 1) {
            // 写时拷贝
            iterator new_start = _alloc.allocate(capacity() + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, pos, new_start);
            new_finish = HxSTL::uninitialized_copy(first, last, new_finish);
            new_finish = HxSTL::uninitialized_copy(pos, _finish, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + capacity());
        } else {
            size_type element_after = _finish - pos;
            iterator old_finish = _finish;
            if (count > element_after) {
                // 插入点之后元素少于待插入元素
                _finish = HxSTL::uninitialized_copy(pos, _finish, _finish + (count - element_after));
                copy_n(first, element_after, pos);
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
    basic_string<CharT, Alloc>::erase_aux(iterator first, iterator last) {
        size_type index = first - _start;
        if (*_used > 1) {
            // 写时拷贝
            iterator new_start = _alloc.allocate(capacity() + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, first);
            new_finish = HxSTL::uninitialized_copy(last, _finish);
            destroy_and_reset(new_start, new_finish, new_start + capacity());
        } else {
            // 向前复制并移除多余元素
            iterator old_finish = _finish;
            _finish = HxSTL::copy(last, _finish, first);
            HxSTL::destroy(_finish, old_finish);
        } 
        return _start + index;
    }

    template <class CharT, class Alloc>
    void basic_string<CharT, Alloc>::append_aux(size_type count, CharT ch, true_type) {
        if (count > _end_of_storage - _finish) {
            // 保留空间不足
            size_type new_sz = 2 * capacity() > size() + count ? 2 * capacity() : size() + count;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            new_finish = HxSTL::uninitialized_fill_n(new_finish, count, ch);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else if (*_used > 1) {
            // 写时拷贝
            iterator new_start = _alloc.allocate(capacity() + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            new_finish = HxSTL::uninitialized_fill_n(new_finish, count, ch);
            destroy_and_reset(new_start, new_finish, new_finish + count);
        } else {
            // 直接填充到末尾
            _finish = HxSTL::uninitialized_fill_n(_finish, count, ch);
            *_finish = 0;
        }
    }

    template <class CharT, class Alloc>
    template <class InputIt>
    void basic_string<CharT, Alloc>::append_aux(InputIt first, InputIt last, false_type) {
        if (count > _end_of_storage - _finish) {
            // 保留空间不足
            size_type new_sz = 2 * capacity() > size() + count ? 2 * capacity() : size() + count;
            iterator new_start = _alloc.allocate(new_sz + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            new_finish = HxSTL::uninitialized_copy(first, last, new_finish);
            destroy_and_reset(new_start, new_finish, new_start + new_sz);
        } else if (*_used > 1) {
            // 写时拷贝
            iterator new_start = _alloc.allocate(capacity() + 1);
            iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
            new_finish = HxSTL::uninitialized_copy(first, last, new_finish);
            destroy_and_reset(new_start, new_finish, new_finish + count);
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
    void basic_string<CharT, Alloc>::reserve(size_type new_cap) {
        if (new_cap > max_size()) throw HxSTL::length_error();
        if (new_cap <= size()) new_cap = size();
        iterator new_start = _alloc.allocate(new_cap + 1);
        iterator new_finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
        destroy_and_reset(new_start, new_finish, new_start + new_cap);
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
