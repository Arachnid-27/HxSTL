#ifndef _SET_H_
#define _SET_H_


#include "rb_tree.h"
#include "functional.h"


namespace HxSTL {

    template <class Key, class Compare = HxSTL::less<Key>, class Alloc = HxSTL::allocator<Key>>
    class multiset {
    public:
        typedef Key                                     key_type;
        typedef Key                                     value_type;
        typedef size_t                                  size_type;
        typedef ptrdiff_t                               difference_type;
        typedef Compare                                 key_compare;
        typedef Compare                                 value_compare;
        typedef Alloc                                   allocator_type;
        typedef value_type&                             reference;
        typedef const value_type&                       const_reference;
        typedef value_type*                             pointer;
        typedef const value_type*                       const_pointer;
    protected:
        typedef HxSTL::rb_tree<key_type, value_type, __identity<value_type>, key_compare, allocator_type>       rep_type;
    public:
        typedef typename rep_type::const_iterator                   iterator;
        typedef typename rep_type::const_iterator                   const_iterator;
        typedef typename HxSTL::reverse_iterator<iterator>          reverse_iterator;
        typedef typename HxSTL::reverse_iterator<const_iterator>    const_reverse_iterator;
    protected:
        rep_type _rep;
    public:
        multiset(): multiset(Compare()) {}

        explicit multiset(const Compare& comp, const Alloc& alloc = Alloc()): _rep(comp, alloc) {}

        template <class InputIt>
        multiset(InputIt first, InputIt last, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
            : _rep(comp, alloc) { insert(first, last); }

        multiset(const multiset& other): _rep(other._rep) {}

        multiset(multiset&& other): _rep(HxSTL::move(other._rep)) {}

        multiset(HxSTL::initializer_list<value_type> init, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
            : _rep(comp, alloc) { insert(init.begin(), init.end()); }

        multiset& operator=(const multiset& other) {
            _rep = other._rep;
            return *this;
        }

        multiset& operator=(multiset&& other) {
            _rep = HxSTL::move(other._rep);
            return *this;
        }

        multiset& operator=(HxSTL::initializer_list<value_type> init) {
            clear();
            insert(init.begin(), init.end());
            return *this;
        }

        Alloc get_allocator() const { return _rep.get_allocator(); }

        iterator begin() noexcept { return _rep.begin(); }

        const_iterator begin() const noexcept { return _rep.begin(); }

        const_iterator cbegin() const noexcept { return _rep.begin(); }

        iterator end() noexcept { return _rep.end(); }

        const_iterator end() const noexcept { return _rep.end(); }

        const_iterator cend() const noexcept { return _rep.end(); }

        reverse_iterator rbegin() noexcept { return reverse_iterator(_rep.end()); }

        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(_rep.end()); }

        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(_rep.end()); }

        reverse_iterator rend() noexcept { return reverse_iterator(_rep.begin()); }

        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(_rep.begin()); }

        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(_rep.begin()); }

        bool empty() const noexcept { return _rep.empty(); }
    
        size_type size() const noexcept { return _rep.size(); }

        size_type max_size() const noexcept { return _rep.max_size(); }

        void clear() { _rep.clear(); }

        iterator insert(const value_type& value) {
            return _rep.insert_equal(value);
        }

        iterator insert(value_type&& value) {
            return _rep.insert_equal(value);
        }

        iterator insert(const_iterator hint, const value_type& value) {
            return _rep.insert_equal(hint, value);
        }

        iterator insert(const_iterator hint, value_type&& value) {
            return _rep.insert_equal(hint, value);
        }

        template <class InputIt>
        void insert(InputIt first, InputIt last) {
            while (first != last) _rep.insert_equal(*(first++));
        }

        void insert(HxSTL::initializer_list<value_type> init) {
            insert(init.begin(), init.end());
        }

        template <class... Args>
        iterator emplace(Args&&... args) {
            return _rep.emplace_equal(HxSTL::forward<Args>(args)...);
        }

        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args) {
            return _rep.emplace_hint_equal(hint, HxSTL::forward<Args>(args)...);
        }
        
        iterator erase(const_iterator pos) { return _rep.erase(pos); }

        iterator erase(const_iterator first, const_iterator last) { return _rep.erase(first, last); }

        size_type erase(const Key& key) { return _rep.erase(key); }

        void swap(multiset& other) { HxSTL::swap(_rep, other._rep); }

        size_type count(const Key& key) const { return _rep.count(key); }

        iterator find(const Key& key) { return _rep.find(key); }

        const_iterator find(const Key& key) const { return _rep.find(key); }

        HxSTL::pair<iterator, iterator> equal_range(const Key& key) { return _rep.equal_range(key); }

        HxSTL::pair<const_iterator, const_iterator> equal_range(const Key& key) const { return _rep.equal_range(key); }

        iterator lower_bound(const Key& key) { return _rep.lower_bound(key); }

        const_iterator lower_bound(const Key& key) const { return _rep.lower_bound(key); }

        iterator upper_bound(const Key& key) { return _rep.upper_bound(key); }

        const_iterator upper_bound(const Key& key) const { return _rep.upper_bound(key); }

        key_compare key_comp() const { return _rep.get_compare(); }

        value_compare value_comp() const { return _rep.get_compare(); }
    public:
        template <class K, class C, class A>
        friend bool operator==(const multiset<K, C, A> &lhs, const multiset<K, C, A> &rhs);
    };

    template <class Key, class Compare, class Alloc>
    bool operator==(const multiset<Key, Compare, Alloc>& lhs, const multiset<Key, Compare, Alloc>& rhs) {
        return lhs._rep == rhs._rep;
    }

    template <class Key, class Compare, class Alloc>
    bool operator!=(const multiset<Key, Compare, Alloc>& lhs, const multiset<Key, Compare, Alloc>& rhs) {
        return !(lhs == rhs);
    }

}


#endif
