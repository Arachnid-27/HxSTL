#ifndef _BASIC_STRING_
#define _BASIC_STRING_


#include "allocator.h"
#include "uninitialized.h"


namespace HxSTL {

    template <class charT, class Alloc = allocator<charT>>
    class basic_string {
    public:
        typedef charT                                       value_type;
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
        template <class InputIterator>
        void initialize_copy_aux(InputIterator first, InputIterator last);
        template <class InputIterator>
        void initialize_copy_n_aux(InputIterator first, size_type count, size_type sz);
        void insert_char_string_aux(size_type index, iterator first, iterator last);
        void reallocate_and_copy(size_type sz);
        void copy_before_write();
    public:
        explicit basic_string(const Alloc& alloc = allocator_type())
            :_ref_count(new long(1)), _start(nullptr), _finish(nullptr), _end_of_storage(nullptr), _alloc(alloc) {}

        basic_string(size_type count, charT ch, const Alloc& alloc = allocator_type())
            :_ref_count(new long(1)), _alloc(alloc) {
                size_type sz = count < 15 ? 15 : count; 
                _start = _alloc.allocate(sz + 1);
                _finish = HxSTL::uninitialized_fill_n(_start, count, ch);
                *_finish = 0;
                _end_of_storage = _start + sz;
            }

        basic_string(const basic_string& other, size_type pos, size_type count = npos, 
            const Alloc& alloc = allocator_type())
            :_ref_count(new long(1)), _alloc(alloc) {
                count = count < other.size() - pos ? count : other.size() - pos; 
                size_type sz = count < 15 ? 15 : count; 
                initialize_copy_n_aux(other._start + pos, count, sz);
            }

        basic_string(const charT* s, size_type count, const Alloc& alloc = allocator_type())
            :_ref_count(new long(1)), _alloc(alloc) {
                size_type sz = count < 15 ? 15 : count; 
                initialize_copy_n_aux(s, count, sz);
            }

        basic_string(const charT* s, const Alloc& alloc = allocator_type())
            :_ref_count(new long(1)), _alloc(alloc) {
                const charT* t = s;
                while (*t) ++t;
                initialize_copy_aux(s, t);
            }

        template <class InputIterator>
        basic_string(InputIterator first, InputIterator last, const Alloc& alloc = allocator_type())
            :_ref_count(new long(1)), _alloc(alloc) { initialize_copy_aux(first, last); }

        basic_string(const basic_string& other)
            :_ref_count(other._ref_count), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(other._alloc) { ++*_ref_count; }

        basic_string(const basic_string& other, const Alloc& alloc)
            :_ref_count(other._ref_count), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(alloc) { ++*_ref_count; }

        basic_string(basic_string&& other)
            :_ref_count(other._ref_count), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(other._alloc) {
                _ref_count = nullptr;
                _start = _finish = _end_of_storage = nullptr; 
            }

        basic_string(basic_string&& other, const Alloc& alloc)
            :_ref_count(other._ref_count), _start(other._start), _finish(other._finish), 
            _end_of_storage(other._end_of_storage), _alloc(alloc) {
                _ref_count = nullptr;
                _start = _finish = _end_of_storage = nullptr; 
            }

        ~basic_string() {
            if (--*_ref_count == 0) delete _ref_count;
        }

        basic_string& operator=(const basic_string& str) {
            basic_string(str).swap(*this);
            return *this;
        }

        basic_string& operator=(basic_string&& str) {
            basic_string(HxSTL::move(str)).swap(*this);
            return *this;
        }

        basic_string& operator=(const charT* s) {
            basic_string(s).swap(*this);
            return *this;
        }

        basic_string& operator=(charT ch) {
            basic_string(1, ch).swap(*this);
            return *this;
        }
    public:
        basic_string& assign(size_type count, charT ch);
        basic_string& assign(const basic_string& str);
        basic_string& assign(basic_string&& str);
        basic_string& assign(const charT* s, size_type count);
        basic_string& assign(const charT* s);
        template <class InputIterator>
        basic_string& assign(InputIterator first, InputIterator last);

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

        const charT& front() const { return operator[](0); }

        reference back() {
            copy_before_write();
            return operator[](size() - 1);
        }

        const charT& back() const { return operator[](size() - 1); }
        
        const charT* data() const { return _start; }

        const charT* c_str() const { return _start; }

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

        basic_string& insert(size_type index, size_type count, charT ch);

        basic_string& insert(size_type index, const charT* s);

        basic_string& insert(size_type index, const charT* s, size_type count);

        basic_string& insert(size_type index, const basic_string& str);

        basic_string& insert(size_type index, const basic_string& str, size_type index_str, size_type count = npos);

        iterator insert(const_iterator pos, charT ch);

        iterator insert(const_iterator pos, size_type count, charT ch);

        template <class InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last);

        basic_string& erase(size_type index = 0, size_type count = npos);

        iterator erase(const_iterator position);

        iterator erase(const_iterator first, const_iterator last);

        void push_back(charT ch);

        void pop_back();

        basic_string& append(size_type count, charT ch);

        basic_string& append(const basic_string& str);

        basic_string& append(const basic_string& str, size_type pos, size_type count = npos);

        basic_string& append(const charT* s, size_type count);

        basic_string& append(const charT* s);

        template <class InputIterator>
        basic_string& append(InputIterator first, InputIterator last);

        basic_string& operator+=(const basic_string& str);

        basic_string& operator+=(charT ch);

        basic_string& operator+=(const charT* s);

        int compare(const basic_string& str) const;

        int compare(size_type pos, size_type count, const basic_string& str) const;

        int compare(size_type pos1, size_type count1, const basic_string& str, size_type pos2, size_type count2) const;

        int compare(const charT* s) const;

        int compare(size_type pos, size_type count, const charT* s) const;

        int compare(size_type pos, size_type count, const charT* s, size_type pos2, size_type count2) const;
    };

    template <class charT, class Alloc>
    template <class InputIterator>
    void basic_string<charT, Alloc>::initialize_copy_aux(InputIterator first, InputIterator last) {
        size_type sz = HxSTL::distance(first, last);
        if (sz < 15) sz = 15;
        _start = _alloc.allocate(sz + 1);
        _finish = HxSTL::uninitialized_copy(first, last, _start);
        *_finish = 0;
        _end_of_storage = _start + sz;
    }

    template <class charT, class Alloc>
    template <class InputIterator>
    void basic_string<charT, Alloc>::initialize_copy_n_aux(InputIterator first, size_type count, size_type sz) {
        _start = _alloc.allocate(sz + 1);
        _finish = HxSTL::uninitialized_copy_n(first, count, _start);
        *_finish = 0;
        _end_of_storage = _start + sz;
    }

    template <class charT, class Alloc>
    void basic_string<charT, Alloc>::insert_char_string_aux(size_type index, iterator first, iterator last) {
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

    template <class charT, class Alloc>
    void basic_string<charT, Alloc>::reallocate_and_copy(size_type sz) {
        iterator new_start = _alloc.allocate(sz + 1);
        _finish = HxSTL::uninitialized_copy(_start, _finish, new_start);
        *_finish = 0;
        _end_of_storage = new_start + sz;
        _start = new_start;
    }

    template <class charT, class Alloc>
    void basic_string<charT, Alloc>::copy_before_write() {
        if (*_ref_count != 1) {
            reallocate_and_copy(capacity());
            --*_ref_count;
            _ref_count = new long(1);
        }
    }

    template <class charT, class Alloc>
    void basic_string<charT, Alloc>::reserve(size_type new_cap) {
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

    template <class charT, class Alloc>
    void basic_string<charT, Alloc>::shrink_to_fit() {
        copy_before_write();
        int unuse = _end_of_storage - _finish;
        if (unuse) {
            _alloc.deallocate(_finish + 1, unuse);
            _end_of_storage = _finish;
        }
    }

    template <class charT, class Alloc>
    basic_string<charT, Alloc>& basic_string<charT, Alloc>::insert(size_type index, size_type count, charT ch) {
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

    template <class charT, class Alloc>
    basic_string<charT, Alloc>& basic_string<charT, Alloc>::insert(size_type index, const charT* s) {
        charT *t = s;
        while (*t) ++t;
        insert_char_string_aux(index, s, t);
        return *this;
    }

    template <class charT, class Alloc>
    basic_string<charT, Alloc>& basic_string<charT, Alloc>::insert(size_type index, const charT* s, size_type count) {
        insert_char_string_aux(index, s, s + count);
        return *this;
    }

    template <class charT, class Alloc>
    basic_string<charT, Alloc>& basic_string<charT, Alloc>::insert(size_type index, const basic_string<charT, Alloc>& str) {
        insert_char_string_aux(index, str.cbegin(), str.cend());
        return *this;
    }

    template <class charT, class Alloc>
    basic_string<charT, Alloc>& basic_string<charT, Alloc>::insert(size_type index, const basic_string<charT, Alloc>& str, 
            size_type index_str, size_type count) {
        if (count == npos) {
            insert_char_string_aux(index, str.cbegin() + index_str, str.cend());
        } else {
            insert_char_string_aux(index, str.cbegin() + index_str, str.cbegin() + index_str + count);
        }
        return *this;
    }

    template <class charT, class Alloc>
    typename basic_string<charT, Alloc>::iterator
    basic_string<charT, Alloc>::insert(const_iterator pos, charT ch) {
    }

    template <class charT, class Alloc>
    typename basic_string<charT, Alloc>::iterator
    basic_string<charT, Alloc>::insert(const_iterator pos, size_type count, charT ch) {
    }

    template <class charT, class Alloc>
    template <class InputIterator>
    typename basic_string<charT, Alloc>::iterator
    basic_string<charT, Alloc>::insert(const_iterator pos, InputIterator first, InputIterator last) {
    }

}

#endif
