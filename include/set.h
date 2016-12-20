#ifndef _SET_H_
#define _SET_H_


#include "rb_tree.h"
#include "functional.h"


namespace HxSTL {

    template <class T>
    struct identity {
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
        explicit set(const Compare& comp = Compare(), const Alloc& alloc = Alloc());

        explicit set(const Alloc& alloc);

        template <class InputIt>
        set(InputIt first, InputIt last, const Compare& comp = Compare(), const Alloc& alloc = Alloc());

        template <class InputIt>
        set(InputIt first, InputIt last, const Alloc& alloc = Alloc());

        set(const set& other);

        set(const set& other, const Alloc& alloc);

        set(set&& other);

        set(set&& other, const Alloc& alloc);

        set(HxSTL::initializer_list<value_type> init, const Compare& comp = Compare(), const Alloc& alloc = Alloc());

        set(HxSTL::initializer_list<value_type> init, const Alloc& alloc = Alloc());

        ~set();

        set& operator=(const set& other);

        set& operator=(set&& other);

        set& operator=(HxSTL::initializer_list<value_type> init);

        Alloc get_allocator() const;

        iterator begin() noexcept;

        const_iterator begin() const noexcept;

        const_iterator cbegin() const noexcept;

        iterator end() noexcept;

        const_iterator end() const noexcept;

        const_iterator cend() const noexcept;

    //  reverse_iterator rbegin() noexcept;

    //  const_reverse_iterator rbegin() const noexcept;

    //  reverse_iterator rend() noexcept;

    //  const_reverse_iterator rend() const noexcept;

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
        HxSTL::pair<iterator, bool> emplace_hint(const_iterator hint, Args&&... args);
        
        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);

        size_type erase(const Key& key);

        void swap(set& other);

        size_type count(const Key& key) const;

        iterator find(const Key& key);

        const_iterator find(const Key& key) const;

        HxSTL::pair<iterator, iterator> equal_range(const Key& key);

        HxSTL::pair<const_iterator, const_iterator> equal_range(const Key& key) const;

        iterator low_bound(const Key& key);

        const_iterator low_bound(const Key& key) const;

        iterator upper_bound(const Key& key);

        const_iterator upper_bound(const Key& key) const;

        key_compare key_comp() const;

        value_compare value_comp() const;
    };

}


#endif
