#ifndef _SET_H_
#define _SET_H_


#include "rb_tree.h"
#include "functional.h"


namespace HxSTL {

    template <class T> struct identity {
        const T& operator()(const T& value) const { return value; }
    };

    template <class Key, class Compare = HxSTL::less<Key>, class Alloc = HxSTL::allocator<Key>>
    class set {
    protected:
        typedef HxSTL::rb_tree<Key, Key, identity<Key>, Compare, Alloc>       rep_type;
    public:
        typedef Key                                     key_type;
        typedef Key                                     value_type;
        typedef size_t                                  size_type;
        typedef ptrdiff_t                               difference_type;
        typedef Compare                                 key_compare;
        typedef Compare                                 value_compare;
        typedef Alloc                                   allocator_type;
        typedef Key&                                    reference;
        typedef const Key&                              const_reference;
        typedef Key*                                    pointer;
        typedef const Key*                              const_pointer;
        typedef typename rep_type::const_iterator       iterator;
        typedef typename rep_type::const_iterator       const_iterator;
    protected:
        rep_type _rep;
    public:
        set(): set(Compare()) {}

        explicit set(const Compare& comp, const Alloc& alloc = Alloc()): _rep(comp, alloc) {}

        explicit set(const Alloc& alloc): _rep(Compare(), alloc) {}

        template <class InputIt>
        set(InputIt first, InputIt last, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
            : _rep(comp, alloc) { insert(first, last); }

        template <class InputIt>
        set(InputIt first, InputIt last, const Alloc& alloc): set(first, last, Compare(), alloc) {}

        set(const set& other): _rep(other) {}

        set(const set& other, const Alloc& alloc): _rep(other, alloc) {}

        set(set&& other): _rep(HxSTL::move(other)) {}

        set(set&& other, const Alloc& alloc): _rep(HxSTL::move(other), alloc) {}

        set(HxSTL::initializer_list<value_type> init, const Compare& comp = Compare(), const Alloc& alloc = Alloc())
            : _rep(comp, alloc) { insert(init.begin(), init.end()); }

        set(HxSTL::initializer_list<value_type> init, const Alloc& alloc): set(init, Compare(), alloc) {}

        ~set();

        set& operator=(const set& other);

        set& operator=(set&& other);

        set& operator=(HxSTL::initializer_list<value_type> init);

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

        void clear();

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
            while (first != last) _rep.insert_unique(first++);
        }

        void insert(HxSTL::initializer_list<value_type> init) {
            insert(init.begin(), init.end());
        }

        template <class... Args>
        HxSTL::pair<iterator, bool> emplace(Args&&... args);

        template <class... Args>
        HxSTL::pair<iterator, bool> emplace_hint(const_iterator hint, Args&&... args);
        
        iterator erase(const_iterator pos) { return _rep.erase(pos); }

        iterator erase(const_iterator first, const_iterator last) { return _rep.erase(first, last); }

        size_type erase(const Key& key) { return _rep.erase(key); }

        void swap(set& other);

        size_type count(const Key& key) const;

        iterator find(const Key& key);

        const_iterator find(const Key& key) const;

        HxSTL::pair<iterator, iterator> equal_range(const Key& key);

        HxSTL::pair<const_iterator, const_iterator> equal_range(const Key& key) const;

        iterator lower_bound(const Key& key) {
            return _rep.lower_bound(key);
        }

        const_iterator lower_bound(const Key& key) const {
            return _rep.lower_bound(key);
        }

        iterator upper_bound(const Key& key) {
            return _rep.upper_bound(key);
        }

        const_iterator upper_bound(const Key& key) const {
            return _rep.upper_bound(key);
        }

        key_compare key_comp() const;

        value_compare value_comp() const;
    };

}


#endif
