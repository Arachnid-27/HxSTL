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
                const Equal& equal = Equal(), const Alloc& alloc = Alloc());

        template <class InputIt>
        unordered_set(InputIt first, InputIt last, size_type bucket = 0, 
                const Hash& hash = Hash(), const Equal& equal = Equal(), const Alloc& alloc = Alloc());

        unordered_set(const unordered_set& other);

        unordered_set(unordered_set&& other);

        unordered_set(HxSTL::initializer_list<value_type> init, size_type bucket = 0, 
                const Hash& hash = Hash(), const Equal& equal = Equal(), const Alloc& alloc = Alloc());
    
        ~unordered_set();

        unordered_set& operator=(const unordered_set& other);

        unordered_set& operator=(unordered_set&& other);

        unordered_set& operator=(HxSTL::initializer_list<value_type> init);

        allocator_type get_allocator() const noexcept;

        iterator begin() noexcept;

        const_iterator begin() const noexcept;

        const_iterator cbegin() const noexcept;
        
        iterator end() noexcept;

        const_iterator end() const noexcept;

        const_iterator cend() const noexcept;

        bool empty() const noexcept;

        size_type size() const noexcept;

        size_type max_size() const noexcept;

        void clear();

        HxSTL::pair<iterator, bool> insert(const value_type& value);

        HxSTL::pair<iterator, bool> insert(value_type&& value);

        iterator insert(const_iterator hint, const value_type& value);

        iterator insert(const_iterator hint, value_type&& value);

        template <class InputIt>
        void insert(InputIt first, InputIt last);

        void insert(HxSTL::initializer_list<value_type> init);

        template <class... Args>
        HxSTL::pair<iterator, bool> emplace(Args&&... args);

        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args);

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);

        size_type erase(const key_type& key);

        void swap(unordered_set& other);

        size_type count(const Key& key) const;

        iterator find(const Key& key);

        const_iterator find(const Key& key) const;

        HxSTL::pair<iterator, iterator> equal_range(const Key& key);

        HxSTL::pair<const_iterator, const_iterator> equal_range(const Key& key) const;
        
        local_iterator begin(size_type n) noexcept;

        const_local_iterator begin(size_type n) const noexcept;

        const_local_iterator cbegin(size_type n) const noexcept;

        local_iterator end(size_type n) noexcept;

        const_local_iterator end(size_type n) const noexcept;

        const_local_iterator cend(size_type n) const noexcept;

        size_type bucket_count() const noexcept;

        size_type max_bucket_count() const noexcept;

        size_type bucket_size(size_type n) const noexcept;

        size_type bucket_size(const Key& key) const noexcept;

        float load_factor() const noexcept;

        float max_load_factor() const noexcept;

        void max_load_factor(float ml);

        void rehash(size_type count);

        void reverse(size_type count);

        Hash hash_function() const noexcept;

        Equal key_eq() const noexcept;
    };

}


#endif
