#ifndef _UNORDERED_SET_
#define _UNORDERED_SET_


#include "hash_table.h"
#include "functional.h"


namespace HxSTL {

    template <class Key, class Hash = HxSTL::hash<Key>, 
             class Equal = HxSTL::equal_to<Key>, class Alloc = HxSTL::allocator<Key>>
    class unordered_set {
    protected:
        typedef HxSTL::hash_table<Key, Key, __identity<Key>, Equal, Hash, Alloc>    rep_type; 
    public:
        typedef Key                                             key_type;
        typedef Key                                             value_type;
        typedef size_t                                          size_type;
        typedef ptrdiff_t                                       difference_type;
        typedef Hash                                            hasher;
        typedef Equal                                           key_equal;
        typedef Alloc                                           allocator_type;
        typedef Key&                                            reference;
        typedef const Key&                                      const_reference;
        typedef Key*                                            pointer;
        typedef const Key*                                      const_pointer;
        typedef typename rep_type::const_iterator               iterator;
        typedef typename rep_type::const_iterator               const_iterator;
        typedef typename rep_type::const_local_iterator         local_iterator;
        typedef typename rep_type::const_local_iterator         const_local_iterator;
    protected:
        rep_type _rep;
    public:
        explicit unordered_set(size_type bucket = 0, const Hash& hash = Hash(), 
                const Equal& equal = Equal(), const Alloc& alloc = Alloc())
            : _rep(bucket, hash, equal, alloc) {}

        template <class InputIt>
        unordered_set(InputIt first, InputIt last, size_type bucket = 0, 
                const Hash& hash = Hash(), const Equal& equal = Equal(), const Alloc& alloc = Alloc())
            : _rep(first, last, bucket, hash, equal, alloc) { insert(first, last); }

        unordered_set(const unordered_set& other): _rep(other._rep) {}

        unordered_set(unordered_set&& other): _rep(HxSTL::move(other._rep)) {}

        unordered_set(HxSTL::initializer_list<value_type> init, size_type bucket = 0, 
                const Hash& hash = Hash(), const Equal& equal = Equal(), const Alloc& alloc = Alloc())
            : unordered_set(init.begin(), init.end(), bucket, hash, equal, alloc) {}

        unordered_set& operator=(const unordered_set& other) {
            _rep = other._rep;
            return *this;
        }

        unordered_set& operator=(unordered_set&& other) {
            _rep = HxSTL::move(other._rep);
            return *this;
        }

        unordered_set& operator=(HxSTL::initializer_list<value_type> init) {
            clear();
            insert(init);
            return *this;
        }

        allocator_type get_allocator() const noexcept { return _rep.get_allocator(); }

        iterator begin() noexcept { return _rep.begin(); }

        const_iterator begin() const noexcept { return _rep.begin(); }

        const_iterator cbegin() const noexcept { return _rep.cbegin(); }
        
        iterator end() noexcept { return _rep.end(); }

        const_iterator end() const noexcept { return _rep.end(); }

        const_iterator cend() const noexcept { return _rep.cend(); }

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
            return _rep.insert_unique(hint, HxSTL::move(value));
        }

        template <class InputIt>
        void insert(InputIt first, InputIt last) {
            while (first != last) {
                _rep.insert_unique(*first++);
            }
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

        size_type erase(const key_type& key) { return _rep.erase(key); }

        void swap(unordered_set& other) { HxSTL::swap(_rep, other._rep); }

        size_type count(const Key& key) const { return _rep.count(key); }

        iterator find(const Key& key) { return _rep.find(key); }

        const_iterator find(const Key& key) const { return _rep.find(key); }

        HxSTL::pair<iterator, iterator> equal_range(const Key& key) { return _rep.equal_range(key); }

        HxSTL::pair<const_iterator, const_iterator> equal_range(const Key& key) const { return _rep.equal_range(key); }
        
        local_iterator begin(size_type n) noexcept { return _rep.begin(n); }

        const_local_iterator begin(size_type n) const noexcept { return _rep.begin(n); }

        const_local_iterator cbegin(size_type n) const noexcept { return _rep.cbegin(n); }

        local_iterator end(size_type n) noexcept { return _rep.end(n); }

        const_local_iterator end(size_type n) const noexcept { return _rep.end(n); }

        const_local_iterator cend(size_type n) const noexcept { return _rep.cend(n); }

        size_type bucket_count() const noexcept { return _rep.bucket_count(); }

        size_type max_bucket_count() const noexcept { return _rep.max_bucket_count(); }

        size_type bucket_size(size_type n) const noexcept { return _rep.bucket_size(n); }

        size_type bucket(const Key& key) const noexcept { return _rep.bucket(key); }

        float load_factor() const noexcept { return _rep.load_factor(); }

        float max_load_factor() const noexcept { return _rep.max_load_factor(); }

        void max_load_factor(float ml) { _rep.max_load_factor(ml); }

        void rehash(size_type count) { _rep.rehash(count); }

        void reverse(size_type count);

        Hash hash_function() const noexcept;

        Equal key_eq() const noexcept;
    public:
        template <class K, class H, class E, class A>
        friend bool operator==(const unordered_set<K, H, E, A> &lhs, const unordered_set<K, H, E, A> &rhs);
    };

    template <class K, class H, class E, class A>
    bool operator==(const unordered_set<K, H, E, A> &lhs, const unordered_set<K, H, E, A> &rhs) {
        return lhs._rep == rhs._rep;
    }

    template <class K, class H, class E, class A>
    bool operator!=(const unordered_set<K, H, E, A> &lhs, const unordered_set<K, H, E, A> &rhs) {
        return !(lhs == rhs);
    }

}


#endif
