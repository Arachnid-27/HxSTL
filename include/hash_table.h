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
            node = node -> next;
            return *this;
        }

        __hash_table_iterator operator++(int) noexcept {
            auto it = *this;
            node = node -> next;
            return it;
        }
    };

    template <class T, class Ref, class Ptr, class Extract, class Hash>
    struct __hash_table_local_iterator: public __hash_table_local_iterator_base {
        typedef T                               value_type;
        typedef Ref                             reference;
        typedef Ptr                             pointer;
        typedef __hash_node<T>*                 link_type;

        __hash_table_local_iterator(size_t bkt, size_t cnt)
            : __hash_table_local_iterator_base(bkt, cnt) {}

        __hash_table_local_iterator(base_link_type x, size_t bkt, size_t cnt)
            : __hash_table_local_iterator_base(x, bkt, cnt) {}

        __hash_table_local_iterator(const __hash_table_local_iterator<T, T&, T*, Extract, Hash>& other)
            : __hash_table_local_iterator_base(other.node, other.bkt, other.cnt) {}

        reference operator*() const noexcept { return static_cast<link_type>(node) -> value; }

        pointer operator->() const noexcept { return &(operator*()); }

        __hash_table_local_iterator& operator++() noexcept {
            increment();
            return *this;
        }

        __hash_table_local_iterator operator++(int) noexcept {
            auto it = *this;
            increment();
            return it;
        }

        void increment() {
            link_type next = static_cast<link_type>(node -> next);
            node = bucket == Hash()(Extract()(next -> value)) ? next : nullptr;
        }
    }

    template <class T, class Ref, class Ptr>
    bool operator==(const __hash_table_iterator<T, Ref, Ptr>& lhs, const __hash_table_iterator<T, Ref, Ptr>& rhs) noexcept {
        return lhs.node == rhs.node;
    }

    template <class T, class Ref, class Ptr>
    bool operator!=(const __hash_table_iterator<T, Ref, Ptr>& lhs, const __hash_table_iterator<T, Ref, Ptr>& rhs) noexcept {
        return lhs.node != rhs.node;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    bool operator==(const __hash_table_iterator<T, RefL, PtrL>& lhs, const __hash_table_iterator<T, RefR, PtrR>& rhs) noexcept {
        return lhs.node == rhs.node;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR>
    bool operator!=(const __hash_table_iterator<T, RefL, PtrL>& lhs, const __hash_table_iterator<T, RefR, PtrR>& rhs) noexcept {
        return lhs.node != rhs.node;
    }

    template <class T, class Ref, class Ptr, class Extract, class Hash>
    bool operator==(const __hash_table_local_iterator<T, Ref, Ptr, Extract, Hash>& lhs, 
            const __hash_table_local_iterator<T, Ref, Ptr, Extract, Hash>& rhs) noexcept {
        return lhs.node == rhs.node;
    }

    template <class T, class Ref, class Ptr, class Extract, class Hash>
    bool operator!=(const __hash_table_local_iterator<T, Ref, Ptr, Extract, Hash>& lhs, 
            const __hash_table_local_iterator<T, Ref, Ptr, Extract, Hash>& rhs) noexcept {
        return lhs.node != rhs.node;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR, class Extract, class Hash>
    bool operator==(const __hash_table_local_iterator<T, RefL, PtrL, Extract, Hash>& lhs, 
            const __hash_table_local_iterator<T, RefR, PtrR, Extract, Hash>& rhs) noexcept {
        return lhs.node == rhs.node;
    }

    template <class T, class RefL, class PtrL, class RefR, class PtrR, class Extract, class Hash>
    bool operator!=(const __hash_table_local_iterator<T, RefL, PtrL, Extract, Hash>& lhs, 
            const __hash_table_local_iterator<T, RefR, PtrR, Extract, Hash>& rhs) noexcept {
        return lhs.node != rhs.node;
    }

    template <class Key, class Value, class Extract, class Equal, class Hash, class Alloc = HxSTL::allocator<__hash_node<Value>>>
    class hash_table {
    public:
        typedef Key                                                                                 key_type;
        typedef Value                                                                               value_type;
        typedef Alloc                                                                               allocator_type;
        typedef Value*                                                                              pointer;
        typedef const Value*                                                                        const_pointer;
        typedef Value&                                                                              reference;
        typedef const Value&                                                                        const_reference;
        typedef size_t                                                                              size_type;
        typedef ptrdiff_t                                                                           difference_type;
        typedef __hash_table_iterator<Value, Value&, Value*>                                        iterator;
        typedef __hash_table_iterator<Value, const Value&, const Value*>                            const_iterator;
        typedef __hash_table_local_iterator<Value, Value&, Value*, Extract, Hash>                   local_iterator;
        typedef __hash_table_local_iterator<Value, const Value&, const Value*, Extract, Hash>       const_local_iterator;
        typedef typename iterator::link_type                                                        bucket_type;
        typedef typename Alloc::template rebind<__hash_node<Value>>::other                          node_allocator_type;
        typedef typename Alloc::template rebind<bucket_type>::other                                 bucket_allocator_type;
    protected:
        float _max_factor;
        bucket_type* _buckets;
        size_type _bucket_count;
        bucket_type _start;
        size_type _count;
        allocator_type _alloc;
        node_allocator_type _node_alloc;
        bucket_allocator_type _bucket_alloc;
    protected:
        size_type BKT_NUM(const Value& value) const { return bucket(Extract()(value)); }
        size_type BKT_NUM(bucket_type bkt) const { return BKT_NUM(bkt -> value); }
        bucket_type NEXT(bucket_type bkt) const noexcept { return static_cast<bucket_type>(bkt -> next); }
        bool EQUAL(const Value& lhs, const Value& rhs) { return Equal()(Extract()(lhs), Extract()(rhs)); }

        template <class... Args>
        bucket_type create_node(Args&&... args);
        void destroy_node(bucket_type node);
        HxSTL::pair<bool, bucket_type> find_node_before(size_type bkt, const Value& value);
        void initialize_aux(size_type count);
        void insert_aux(size_type bkt, bucket_type node);
        size_type erase_aux(const Value& value);
        void clear_aux();
    public:
        hash_table(): _count(0), _alloc(allocator_type()), 
            _node_alloc(node_allocator_type()), _bucket_alloc(bucket_allocator_type()) {
            initialize_aux(0);
        }

        ~hash_table() {
            if (_buckets) {
                clear_aux();
                _bucket_alloc.deallocate(_buckets, _bucket_count);
            }
        }

        iterator begin() noexcept { return _start; }

        const_iterator begin() const noexcept { return _start; }

        const_iterator cbegin() const noexcept { return _start; }

        iterator end() noexcept { return nullptr; }

        const_iterator end() const noexcept { return nullptr; }

        const_iterator cend() const noexcept { return nullptr; }

        size_type size() const noexcept { return _count; }

        size_type bucket_count() const noexcept { return _bucket_count; }

        bool empty() const noexcept { return _count == 0; }

        void clear() {
            clear_aux();
            _count = 0;
            _start = nullptr;
        }

        HxSTL::pair<iterator, bool> insert_unique(const Value& value);

        iterator erase(const_iterator pos) {
            iterator it = *pos++;
            erase_aux(*it);
            return iterator(pos.node);
        }

        iterator erase(const_iterator first, const_iterator last) {
            if (first == begin() && last == end()) {
                clear();
            } else {
                while (first != last) erase_aux(*first++);
            }

            return last;
        }

        size_type erase(const Value& value) { return erase_aux(value); }

        local_iterator begin(size_type n) noexcept {
            return local_iterator(_buckets[n], n, _bucket_count);
        }

        const_local_iterator begin(size_type n) const noexcept {
            return const_local_iterator(_buckets[n], n, _bucket_count);
        }

        const_local_iterator cbegin(size_type n) const noexcept {
            return const_local_iterator(_buckets[n], n, _bucket_count);
        }

        local_iterator end(size_type n) noexcept {
            return local_iterator(n, _bucket_count);
        }

        const_local_iterator end(size_type n) const noexcept {
            return const_local_iterator(n, _bucket_count);
        }

        const_local_iterator cend(size_type n) const noexcept {
            return const_local_iterator(n, _bucket_count);
        }

        float load_factor() const noexcept {
            return static_cast<float>(_count) / static_cast<float>(_bucket_count);
        }

        size_type bucket(const Key& key) const { return Hash()(key) % _bucket_count; }

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
        _bucket_count = __prime_hash_policy::next_buckets(count);
        _buckets = _bucket_alloc.allocate(_bucket_count);
        _start = nullptr;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::insert_aux(size_type bkt, bucket_type node) {
        node -> next = _buckets[bkt] ? _buckets[bkt] : _start;
        _buckets[bkt] = node;
        _start = node;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::clear_aux() {
        for (iterator first = begin(), last = end(); first != last; ++first) {
            destroy_node(static_cast<bucket_type>((first++).node));
        }
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::erase_aux(const V& value) -> size_type {
        size_type count = _count;
        size_type bkt = BKT_NUM(value);

        bucket_type prev = nullptr;
        bucket_type cur = _buckets[bkt];

        while (cur && BKT_NUM(cur) == bkt) {
            bucket_type next = NEXT(cur);
            if (EQUAL(value, cur -> value)) {
                if (prev == nullptr) {
                    _buckets[bkt] = next;
                } else {
                    cur -> next = next;
                }
                destroy_node(cur);
                --_count;
            } else {
                prev = cur;
            }
            cur = next;
        }

        return count - _count;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::find_node_before(size_type bkt, const V& value) -> HxSTL::pair<bool, bucket_type> {
        bucket_type prev = nullptr;
        bucket_type cur = _buckets[bkt];

        while (cur && BKT_NUM(cur) == bkt) {
            if (EQUAL(value, cur -> value)) {
                return HxSTL::pair<bool, bucket_type>(true, prev);
            }
            prev = cur;
            cur = NEXT(cur);
        }

        return HxSTL::pair<bool, bucket_type>(false, nullptr);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::insert_unique(const V& value) -> HxSTL::pair<iterator, bool> {
        rehash(_count + 1);

        size_type bkt = BKT_NUM(value);

        for (bucket_type cur = _buckets[bkt]; cur && BKT_NUM(cur) == bkt; cur = NEXT(cur)) {
            if (EQUAL(value, cur -> value)) {
                return HxSTL::pair<iterator, bool>(cur, false);
            }
        }
        
        bucket_type node = create_node(value);
        insert_aux(bkt, node);
        ++_count;
        return HxSTL::pair<iterator, bool>(node, true);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::rehash(size_type count) {
        if (_bucket_count < count) {
            // 暂不考虑异常
            _bucket_alloc.deallocate(_buckets, _bucket_count);

            _bucket_count = __prime_hash_policy::next_buckets(count);
            _buckets = _bucket_alloc.allocate(_bucket_count);

            bucket_type first = _start;
            _start = nullptr;
            while (first != nullptr) {
                bucket_type node = first;
                first = NEXT(first);
                size_type bkt = BKT_NUM(node);
                insert_aux(bkt, node);
            }
        }
    }

}


#endif
