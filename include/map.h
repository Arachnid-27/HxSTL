#ifndef _MAP_H_
#define _MAP_H_


#include "rb_tree.h"
#include "exception.h"
#include "functional.h"


namespace HxSTL {

    template <class Key, class T, class Compare = HxSTL::less<Key>, 
             class Alloc = HxSTL::allocator<HxSTL::pair<const Key, T>>>
    class map {
    public:
        typedef Key                                     key_type;
        typedef T                                       mapped_type;
        typedef HxSTL::pair<const Key, T>               value_type;
        typedef size_t                                  size_type;
        typedef ptrdiff_t                               difference_type;
        typedef Compare                                 key_compare;
        typedef Alloc                                   allocator_type;
        typedef value_type&                             reference;
        typedef const value_type&                       const_reference;
        typedef value_type*                             pointer;
        typedef const value_type*                       const_pointer;
    protected:
        typedef HxSTL::rb_tree<key_type, value_type, __select1st<value_type>, key_compare, allocator_type>       rep_type;
    public:
        typedef typename rep_type::iterator                             iterator;
        typedef typename rep_type::const_iterator                       const_iterator;
        typedef typename HxSTL::reverse_iterator<iterator>              reverse_iterator;
        typedef typename HxSTL::reverse_iterator<const_iterator>        const_reverse_iterator;

        class value_compare {
        protected:
            Compare _compare;

            value_compare(Compare comp): _compare(comp) {}
        public:
            bool operator()(const value_type& lhs, const value_type& rhs) const noexcept {
                return _compare(lhs.first, rhs.first);
            }
        };
    protected:
        rep_type _rep;
    public:
        map(): map(Compare()) {}

        explicit map(const Compare& comp, const Alloc& alloc = Alloc()): _rep(comp, alloc) {}

        template <class InputIt>
        map(InputIt first, InputIt last, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
            : _rep(comp, alloc) { insert(first, last); }

        map(const map& other): _rep(other._rep) {}

        map(map&& other): _rep(HxSTL::move(other._rep)) {}

        map(HxSTL::initializer_list<value_type> init, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
            : map(init.begin(), init.end(), comp, alloc) {}

        map& operator=(const map& other) {
            _rep = other._rep;
            return *this;
        }

        map& operator=(map&& other) {
            _rep = HxSTL::move(other._rep);
            return *this;
        }

        map& operator=(HxSTL::initializer_list<value_type> init) {
            clear();
            insert(init);
            return *this;
        }

        Alloc get_allocator() const noexcept { return _rep.get_allocator(); }

        T& at(const Key& key) {
            iterator it = _rep.find(key);
            if (it == end()) throw HxSTL::out_of_range();
            return it -> second;
        }

        const T& at(const Key& key) const {
            const_iterator it = _rep.find(key);
            if (it == end()) throw HxSTL::out_of_range();
            return it -> second;
        }

        T& operator[](const Key& key) {
            iterator it = _rep.find(key);
            if (it != end()) return it -> second;
            return insert(HxSTL::make_pair(key, T())).first -> second;
        }

        T& operator[](Key&& key) {
            iterator it = _rep.find(key);
            if (it != end()) return it -> second;
            return insert(HxSTL::make_pair(HxSTL::move(key), T())).first -> second;
        }

        iterator begin() noexcept { return _rep.begin(); }

        const_iterator begin() const noexcept { return _rep.begin(); }

        const_iterator cbegin() const noexcept { return _rep.begin(); }

        iterator end() noexcept { return _rep.end(); }

        const_iterator end() const noexcept { return _rep.end(); }

        const_iterator cend() const noexcept { return _rep.end(); }

        reverse_iterator rbegin() noexcept { return reverse_iterator(_rep.end()); }

        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(_rep.end()); }

        reverse_iterator rend() noexcept { return reverse_iterator(_rep.begin()); }

        const_reverse_iterator rend() const noexcept { return reverse_iterator(_rep.begin()); }

        bool empty() const noexcept { return _rep.empty(); }
    
        size_type size() const noexcept { return _rep.size(); }

        size_type max_size() const noexcept { return _rep.max_size(); }

        void clear() { _rep.clear(); }

        HxSTL::pair<iterator, bool> insert(const value_type& value) {
            return _rep.insert_unique(value);
        }

        HxSTL::pair<iterator, bool> insert(value_type&& value) {
            return _rep.insert_unique(HxSTL::move(value));
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

        void swap(map& other) { HxSTL::swap(_rep, other._rep); }

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

        value_compare value_comp() const { return value_compare(); }
    public:
        template <class K, class V, class C, class A>
        friend bool operator==(const map<K, V, C, A> &lhs, const map<K, V, C, A> &rhs);
    };

    template <class Key, class T, class Compare, class Alloc>
    bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
        return lhs._rep == rhs._rep;
    }

    template <class Key, class T, class Compare, class Alloc>
    bool operator!=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
        return !(lhs == rhs);
    }

}


#endif
