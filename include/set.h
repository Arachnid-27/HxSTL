#ifndef _SET_H_
#define _SET_H_


#include "rb_tree.h"
#include "functional.h"


namespace HxSTL {

    template <class Key, class Compare = HxSTL::less<Key>, class Alloc = HxSTL::allocator<Key>>
    class set {
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
        typedef typename rep_type::const_iterator       iterator;
        typedef typename rep_type::const_iterator       const_iterator;
    protected:
        rep_type _rep;
    public:
        set(): set(Compare()) {}

        explicit set(const Compare& comp, const Alloc& alloc = Alloc()): _rep(comp, alloc) {}

        template <class InputIt>
        set(InputIt first, InputIt last, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
            : _rep(comp, alloc) { insert(first, last); }

        set(const set& other): _rep(other._rep) {}

        set(set&& other): _rep(HxSTL::move(other._rep)) {}

        set(HxSTL::initializer_list<value_type> init, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
            : _rep(comp, alloc) { insert(init.begin(), init.end()); }

        set& operator=(const set& other) {
            _rep = other._rep;
            return *this;
        }

        set& operator=(set&& other) {
            _rep = HxSTL::move(other._rep);
            return *this;
        }

        set& operator=(HxSTL::initializer_list<value_type> init) {
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

    //  reverse_iterator rbegin() noexcept;

    //  const_reverse_iterator rbegin() const noexcept;

    //  reverse_iterator rend() noexcept;

    //  const_reverse_iterator rend() const noexcept;

        bool empty() const noexcept { return _rep.empty(); }
    
        size_type size() const noexcept { return _rep.size(); }

        size_type max_size() const noexcept { return _rep.max_size(); }

        void clear() { _rep.clear(); }

        HxSTL::pair<iterator, bool> insert(const value_type& value) {
            return _rep.insert_unique(value);
        }

        HxSTL::pair<iterator, bool> insert(value_type&& value) {
            return _rep.insert_unique(value);
        }

        iterator insert(const_iterator hint, const value_type& value) {
            return _rep.insert_unique(hint, value);
        }

        iterator insert(const_iterator hint, value_type&& value) {
            return _rep.insert_unique(hint, value);
        }

        template <class InputIt>
        void insert(InputIt first, InputIt last) {
            while (first != last) _rep.insert_unique(*(first++));
        }

        void insert(HxSTL::initializer_list<value_type> init) {
            insert(init.begin(), init.end());
        }

        template <class... Args>
        HxSTL::pair<iterator, bool> emplace(Args&&... args) {
            return _rep.emplace_unique(HxSTL::forward<Args>(args)...);
        }

        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args) {
            return _rep.emplace_hint_unique(hint, HxSTL::forward<Args>(args)...);
        }
        
        iterator erase(const_iterator pos) { return _rep.erase(pos); }

        iterator erase(const_iterator first, const_iterator last) { return _rep.erase(first, last); }

        size_type erase(const Key& key) { return _rep.erase(key); }

        void swap(set& other) { HxSTL::swap(_rep, other._rep); }

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
        friend bool operator==(const set<K, C, A> &lhs, const set<K, C, A> &rhs);
    };

    template <class Key, class Compare, class Alloc>
    bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
        return lhs._rep == rhs._rep;
    }

    template <class Key, class Compare, class Alloc>
    bool operator!=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
        return !(lhs == rhs);
    }

}


#endif
