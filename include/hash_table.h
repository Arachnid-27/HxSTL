#ifndef _HASH_TABLE_
#define _HASH_TABLE_


#include "allocator.h"
#include "hash_table_base.h"


namespace HxSTL {

    template <class T>
    struct __hash_node: public __hash_node_base {
        T value;
    };

    template <class T, class Ref, class Ptr>
    struct __hash_table_iterator: public __hash_table_iterator_base {
        typedef HxSTL::forward_iterator_tag     iterator_category;
        typedef T                               value_type;
        typedef Ref                             reference;
        typedef Ptr                             pointer;
        typedef __hash_node<T>*                 link_type;

        __hash_table_iterator() {}

        __hash_table_iterator(base_link_type x): __hash_table_iterator_base(x) {}

        __hash_table_iterator(const __hash_table_iterator<T, T&, T*>& other): __hash_table_iterator_base(other.node) {}

        reference operator*() const noexcept { return static_cast<link_type>(node) -> value; }

        pointer operator->() const noexcept { return &(operator*()); }

        __hash_table_iterator& operator++() noexcept {
            increment();
            return *this;
        }

        __hash_table_iterator operator++(int) noexcept {
            __hash_table_iterator it = *this;
            increment();
            return it;
        }
    };

    template <class T, class Ref, class Ptr>
    bool operator==(const __hash_table_iterator<T, Ref, Ptr>& lhs, const __hash_table_iterator<T, Ref, Ptr>& rhs) {
        return lhs.node == rhs.node;
    }

    template <class T, class Ref, class Ptr>
    bool operator!=(const __hash_table_iterator<T, Ref, Ptr>& lhs, const __hash_table_iterator<T, Ref, Ptr>& rhs) {
        return lhs.node != rhs.node;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    bool operator==(const __hash_table_iterator<T, RefL, PtrL>& lhs, const __hash_table_iterator<T, RefR, PtrR>& rhs) {
        return lhs.node == rhs.node;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    bool operator!=(const __hash_table_iterator<T, RefL, PtrL>& lhs, const __hash_table_iterator<T, RefR, PtrR>& rhs) {
        return lhs.node != rhs.node;
    }

    template <class Key, class Value, class Extract, class Equal, class Hash, class Alloc = HxSTL::allocator<__hash_node<Value>>>
    class hash_table {
    public:
        typedef Key                                                                 key_type;
        typedef Value                                                               value_type;
        typedef Alloc                                                               allocator_type;
        typedef Value*                                                              pointer;
        typedef const Value*                                                        const_pointer;
        typedef Value&                                                              reference;
        typedef const Value&                                                        const_reference;
        typedef size_t                                                              size_type;
        typedef ptrdiff_t                                                           difference_type;
        typedef __hash_table_iterator<Value, Value&, Value*>                        iterator;
        typedef __hash_table_iterator<Value, const Value&, const Value*>            const_iterator;
        typedef typename iterator::link_type                                        bucket_type;
        typedef typename iterator::base_link_type                                   base_bucket_type;
        typedef typename Alloc::template rebind<__hash_node<Value>>::other          node_allocator_type;
        typedef typename Alloc::template rebind<__hash_node<bucket_type>>::other    bucket_allocator_type;
    protected:
        bucket_type* _buckets;
        size_type _buckets_size;
        bucket_type _start;
        size_type _count;
        allocator_type _alloc;
        node_allocator_type _node_alloc;
        bucket_allocator_type _bucket_alloc;
    protected:
        size_type BKT_NUM(const Value& value) const noexcept { return Hash()(Extract()(value)) % _buckets_size; }
        size_type BKT_NUM(bucket_type bkt) const noexcept { return BKT_NUM(bkt -> value); }
        bucket_type NEXT(bucket_type bkt) const noexcept { return static_cast<bucket_type>(bkt -> next); }
        bool EQUAL(const Value& lhs, const Value& rhs) { return Equal()(Extract()(lhs), Extract(rhs)); }

        template <class... Args>
        bucket_type create_node(Args&&... args);
        void destroy_node(bucket_type node);
        void initialize_aux(size_type count);
    public:
        hash_table(): _count(0), _alloc(allocator_type()), 
            _node_alloc(node_allocator_type()), _bucket_alloc(bucket_allocator_type()) {
            initialize_aux(0);
        }

        iterator begin() { return _buckets[index]; }

        const_iterator begin() const { return _buckets[index]; }

        const_iterator cbegin() const { return _buckets[index]; }

        iterator end() { return nullptr; }

        const_iterator end() const { return nullptr; }

        const_iterator cend() const { return nullptr; }

        HxSTL::pair<iterator, bool> insert_unique(const Value& value);

        void rehash(size_type count);
    };

    template <class K, class V, class Ex, class Eq, class H, class A>
    template <class... Args>
    auto hash_table<K, V, Ex, Eq, H, A>::create_node(Args&&... args) -> bucket_type {
        bucket_type node = _node_alloc.allocate(1);
        _node_alloc.construct(&(node -> value), HxSTL::forward<Args>(args)...);
        node -> next = nullptr;
        return node;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::destroy_node(bucket_type node) {
        _node_alloc.destroy(&(node -> value));
        _node_alloc.deallocate(node, 1);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::initialize_aux(size_type count) {
        _buckets_size = __prime_hash_policy::next_buckets(count);
        _buckets = _bucket_alloc.allocate(_buckets_size);
        _start = _buckets[_buckets_size];
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::insert_unique(const V& value) -> HxSTL::pair<iterator, bool> {
        size_type bkt = BKT_NUM(value);
        bucket_type first = _buckets[bkt];

        for (bucket_type cur = first; cur && BKT_NUM(cur) == index; cur = NEXT(cur)) {
            if (EQUAL(value, cur -> value)) {
                return HxSTL::pair<iterator, bool>(cur, false);
            }
        }
        
        bucket_type node = create_node(value);
        node -> next = first ? first : _start;
        _buckets[bkt] = node;
        _start = node;
        ++_count;
        return HxSTL::pair<iterator, bool>(node, true);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::rehash(size_type count) {
        if (_buckets_size < count) {
            size_type new_buckets_size = __prime_hash_policy::next_buckets(count);
            bucket_type* new_buckets = _bucket_alloc.allocate(new_buckets_size);

        }
    }

}


#endif
