#ifndef _HASH_TABLE_
#define _HASH_TABLE_


#include "allocator.h"
#include "hash_table_base.h"
#include <iostream>


namespace HxSTL {

    template <class T>
    struct __hash_node: public __hash_node_base {
        T value;
    };

    template <class T, class Ref, class Ptr, class Extract, class Hash>
    struct __hash_table_local_iterator: public __hash_table_local_iterator_base {
        typedef T                               value_type;
        typedef Ref                             reference;
        typedef Ptr                             pointer;
        typedef size_t                          size_type;
        typedef ptrdiff_t                       difference_type;
        typedef __hash_node<T>*                 link_type;

        __hash_table_local_iterator(size_t bkt, size_t cnt)
            : __hash_table_local_iterator_base(bkt, cnt) {}

        __hash_table_local_iterator(base_link_type x, size_t bkt, size_t cnt)
            : __hash_table_local_iterator_base(x, bkt, cnt) {}

        __hash_table_local_iterator(const __hash_table_local_iterator<T, T&, T*, Extract, Hash>& other)
            : __hash_table_local_iterator_base(other.node, other.bucket, other.bucket_count) {}

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
            node = next && bucket == Hash()(Extract()(next -> value)) ? next : nullptr;
        }
    };

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

    template <class T, class Ref, class Ptr>
    struct __hash_table_iterator: public __hash_table_iterator_base {
        typedef T                               value_type;
        typedef Ref                             reference;
        typedef Ptr                             pointer;
        typedef size_t                          size_type;
        typedef ptrdiff_t                       difference_type;
        typedef __hash_node<T>*                 link_type;

        __hash_table_iterator() {}

        __hash_table_iterator(base_link_type x): __hash_table_iterator_base(x) {}

        __hash_table_iterator(const __hash_table_iterator<T, T&, T*>& other): __hash_table_iterator_base(other.node) {}

        template <class Extract, class Hash>
        __hash_table_iterator(const __hash_table_local_iterator<T, Ref, Ptr, Extract, Hash>& other)
        : __hash_table_iterator_base(other.node) {}

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

    template <class Key, class Value, class Extract, class Equal, class Hash, class Alloc = HxSTL::allocator<__hash_node<Value>>>
    class hash_table {
    public:
        typedef Key                                                                                 key_type;
        typedef Value                                                                               value_type;
        typedef Alloc                                                                               allocator_type;
        typedef Hash                                                                                hasher;
        typedef Equal                                                                               key_equal;
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
        size_type _count;
        size_type _bucket_count;
        bucket_type* _buckets;
        bucket_type _start;
        hasher _hash;
        key_equal _equal;
        allocator_type _alloc;
        node_allocator_type _node_alloc;
        bucket_allocator_type _bucket_alloc;
    protected:
        size_type BKT_NUM(const Value& value) const { return bucket(Extract()(value)); }
        size_type BKT_NUM(bucket_type bkt) const { return BKT_NUM(bkt -> value); }
        bucket_type NEXT(bucket_type bkt) const noexcept { return static_cast<bucket_type>(bkt -> next); }
        bool EQUAL(const Value& lhs, const Value& rhs) { return _equal(Extract()(lhs), Extract()(rhs)); }

        template <class... Args>
        bucket_type create_node(Args&&... args);
        void destroy_node(bucket_type node);
        bucket_type find_node(size_type bkt, const Value& value);
        bucket_type find_node_before(size_type bkt, bucket_type node);
        void initialize_aux(size_type count);
        void copy_aux(bucket_type first);
        void insert_aux(size_type bkt, bucket_type node);
        void insert_aux(bucket_type pos, bucket_type node);
        void clear_aux();
    public:
        hash_table(size_type bucket, const Hash& hash, const Equal& equal, const Alloc& alloc)
            : _max_factor(1.0), _count(0), _hash(hash), _equal(equal), _alloc(alloc) {
                initialize_aux(bucket);
            }

        template <class InputIt>
        hash_table(InputIt first, InputIt last, size_type bucket, const Hash& hash, const Equal& equal, const Alloc& alloc)
            : _max_factor(1.0), _count(0), _hash(hash), _equal(equal), _alloc(alloc) {
                size_type n = HxSTL::distance(first, last);
                if (bucket < n) bucket = n;
                initialize_aux(bucket);
            }

        hash_table(const hash_table& other): _max_factor(other._max_factor), _count(other._count),
            _bucket_count(other._bucket_count), _hash(other._hash), _equal(other._equal), _alloc(other._alloc) {
                initialize_aux(_bucket_count);
                copy_aux(other._start);
            }

        hash_table(hash_table&& other): _max_factor(other._max_factor), _count(other._count),
            _buckets(other._buckets), _bucket_count(other._bucket_count), _start(other._start),
            _hash(HxSTL::move(other._hash)), _equal(HxSTL::move(other._equal)), _alloc(HxSTL::move(other._alloc)) {
                other._buckets = nullptr;
            }

        ~hash_table() {
            if (_buckets) {
                clear_aux();
                _bucket_alloc.deallocate(_buckets, _bucket_count);
            }
        }

        hash_table& operator=(const hash_table& other) {
            hash_table(other).swap(*this);
            return *this;
        }

        hash_table& operator=(hash_table&& other) {
            hash_table(HxSTL::move(other)).swap(*this);
            return *this;
        }

        Alloc get_allocator() const noexcept { return _alloc; }

        iterator begin() noexcept { return _start; }

        const_iterator begin() const noexcept { return _start; }

        const_iterator cbegin() const noexcept { return _start; }

        iterator end() noexcept { return nullptr; }

        const_iterator end() const noexcept { return nullptr; }

        const_iterator cend() const noexcept { return nullptr; }

        size_type size() const noexcept { return _count; }

        bool empty() const noexcept { return _count == 0; }

        void clear() {
            clear_aux();
            _count = 0;
            _start = nullptr;
        }

        void swap(hash_table& other) {
            HxSTL::swap(_max_factor, other._max_factor);
            HxSTL::swap(_count, other._count);
            HxSTL::swap(_bucket_count, other._bucket_count);
            HxSTL::swap(_buckets, other._buckets);
            HxSTL::swap(_start, other._start);
            HxSTL::swap(_hash, other._hash);
            HxSTL::swap(_equal, other._equal);
        }

        template <class T>
        iterator insert_equal(T&& value);

        template <class T>
        iterator insert_equal(const_iterator hint, T&& value);

        template <class T>
        HxSTL::pair<iterator, bool> insert_unique(T&& value);

        template <class T>
        iterator insert_unique(const_iterator, T&& value) {
            return insert_unique(HxSTL::forward<T>(value)).first;
        }

        template <class... Args>
        iterator emplace_equal(Args&&... args);

        template <class... Args>
        iterator emplace_hint_equal(const_iterator hint, Args&&... args);

        template <class... Args>
        HxSTL::pair<iterator, bool> emplace_unique(Args&&... args);

        template <class... Args>
        iterator emplace_hint_unique(const_iterator, Args&&... args) {
            return insert_unique(HxSTL::forward<Args>(args)...).first;
        }

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);

        size_type erase(const Value& value);

        size_type count(const Key& key) const noexcept {
            const size_type bkt = bucket(key);
            return HxSTL::count_if(begin(bkt), end(bkt), [&key, this] (const int &val) { return this -> _equal(key, Extract()(val)); });
        }

        iterator find(const Key& key) {
            const size_type bkt = bucket(key);
            return HxSTL::find_if(begin(bkt), end(bkt), [&key, this] (const int &val) { return this -> _equal(key, Extract()(val)); });
        }

        const_iterator find(const Key& key) const {
            const size_type bkt = bucket(key);
            return HxSTL::find_if(begin(bkt), end(bkt), [&key, this] (const int &val) { return this -> _equal(key, Extract()(val)); });
        }

        HxSTL::pair<iterator, iterator> equal_range(const Key& key) {
            iterator it1 = find(key);
            iterator it2 = HxSTL::find_if_not(it1, end(),
                    [&key, this] (const int &val) { return this -> _equal(key, Extract()(val)); });
            return HxSTL::pair<iterator, iterator>(it1, it2);
        }

        HxSTL::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
            const_iterator it1 = find(key);
            const_iterator it2 = HxSTL::find_if_not(it1, end(),
                    [&key, this] (const int &val) { return this -> _equal(key, Extract()(val)); });
            return HxSTL::pair<const_iterator, const_iterator>(it1, it2);
        }

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

        size_type bucket_count() const noexcept { return _bucket_count; }

        size_type max_bucket_count() const noexcept {
            return __prime_hash_policy::max_buckets();
        }

        size_type bucket_size(size_type n) const noexcept {
            return HxSTL::distance(begin(n), end(n));
        }

        size_type bucket(const Key& key) const { return _hash(key) % _bucket_count; }

        float load_factor() const noexcept {
            return static_cast<float>(_count) / static_cast<float>(_bucket_count);
        }

        float max_load_factor() const noexcept { return _max_factor; }

        void max_load_factor(float ml) noexcept { _max_factor = ml; }

        void rehash(size_type count);
    };

    template <class K, class V, class Ex, class Eq, class H, class A>
    bool operator==(const hash_table<K, V, Ex, Eq, H, A>& lhs, const hash_table<K, V, Ex, Eq, H, A>& rhs) noexcept {
        return lhs.size() == rhs.size() && HxSTL::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    bool operator!=(const hash_table<K, V, Ex, Eq, H, A>& lhs, const hash_table<K, V, Ex, Eq, H, A>& rhs) noexcept {
        return !(lhs == rhs);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    template <class... Args>
    auto hash_table<K, V, Ex, Eq, H, A>::create_node(Args&&... args) -> bucket_type {
        bucket_type node = _node_alloc.allocate(1);
        _node_alloc.construct(&(node -> value), HxSTL::forward<Args>(args)...);
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
        memset(_buckets, 0, _bucket_count * sizeof(bucket_type*));
        _start = nullptr;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::copy_aux(bucket_type first) {
        bucket_type cur;
        size_type cur_bkt;

        if (first != nullptr) {
            cur = create_node(first -> value);
            cur_bkt = BKT_NUM(cur); 
            _start = cur;
            _buckets[cur_bkt] = cur;
            first = NEXT(first);
        }

        while (first != nullptr) {
            bucket_type node = create_node(first -> value);
            size_type node_bkt = BKT_NUM(node);

            if (cur_bkt != node_bkt) {
                _buckets[node_bkt] = node;
                cur_bkt = node_bkt;
            }

            cur -> next = node;
            cur = node;
            first = NEXT(first);
        }

        cur -> next = nullptr;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::insert_aux(size_type bkt, bucket_type node) {
        if (_buckets[bkt]) {
            node -> next = _buckets[bkt] -> next;
            _buckets[bkt] -> next = node;
        } else {
            node -> next = _start;
            _buckets[bkt] = node;
            _start = node;
        }
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::insert_aux(bucket_type pos, bucket_type node) {
        node -> next = pos -> next;
        pos -> next = node;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::clear_aux() {
        iterator first = begin();
        iterator last = end();
        while (first != last) {
            destroy_node(static_cast<bucket_type>((first++).node));
        }
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::find_node_before(size_type bkt, bucket_type node) -> bucket_type {
        bucket_type prev = nullptr;
        bucket_type cur = _buckets[bkt];

        if (cur == node) {
            cur = _start;
        }

        while (cur != node) {
            prev = cur;
            cur = NEXT(cur);
        }

        return prev;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::find_node(size_type bkt, const V& value) -> bucket_type {
        for (bucket_type cur = _buckets[bkt]; cur && BKT_NUM(cur) == bkt; cur = NEXT(cur)) {
            if (EQUAL(value, cur -> value)) {
                return cur;
            }
        }
        return nullptr;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    template <class T>
    auto hash_table<K, V, Ex, Eq, H, A>::insert_equal(T&& value) -> iterator {
        rehash(_count + 1);

        bucket_type node = create_node(HxSTL::forward<T>(value));
        size_type bkt = BKT_NUM(value);
        bucket_type temp = find_node(bkt, node -> value);

        if (temp) {
            insert_aux(temp, node);
        } else {
            insert_aux(bkt, node);
        }

        ++_count;
        return node;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    template <class T>
    auto hash_table<K, V, Ex, Eq, H, A>::insert_equal(const_iterator hint, T&& value) -> iterator {
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    template <class T>
    auto hash_table<K, V, Ex, Eq, H, A>::insert_unique(T&& value) -> HxSTL::pair<iterator, bool> {
        size_type bkt = BKT_NUM(value);
        bucket_type node = find_node(bkt, value);

        if (node) {
            return HxSTL::pair<iterator, bool>(node, false);
        }

        rehash(_count + 1);

        node = create_node(HxSTL::forward<T>(value));
        insert_aux(bkt, node);
        ++_count;
        return HxSTL::pair<iterator, bool>(node, true);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    template <class... Args>
    auto hash_table<K, V, Ex, Eq, H, A>::emplace_equal(Args&&... args) -> iterator {
        rehash(_count + 1);

        bucket_type node = create_node(HxSTL::forward<Args>(args)...);
        size_type bkt = BKT_NUM(node -> value);
        bucket_type temp = find_node(bkt, node -> value);

        if (temp) {
            insert_aux(temp, node);
        } else {
            insert_aux(bkt, node);
        }

        ++_count;
        return node;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    template <class... Args>
    auto hash_table<K, V, Ex, Eq, H, A>::emplace_unique(Args&&... args) -> HxSTL::pair<iterator, bool> {
        bucket_type node = create_node(HxSTL::forward<Args>(args)...);
        size_type bkt = BKT_NUM(node -> value);
        bucket_type temp = find_node(bkt, node -> value);

        if (temp) {
            return HxSTL::pair<iterator, bool>(temp, false);
        }

        rehash(_count + 1);

        insert_aux(bkt, node);
        ++_count;
        return HxSTL::pair<iterator, bool>(node, true);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::erase(const_iterator pos) -> iterator {
        bucket_type node = static_cast<bucket_type>((pos++).node);

        size_type bkt = BKT_NUM(node -> value);
        bucket_type prev = find_node_before(bkt, node);

        _buckets[bkt] = static_cast<bucket_type>(node -> next);
        if (prev) {
            prev -> next = node -> next;
            destroy_node(node);
        } else {
            _start = _buckets[bkt];
        }

        --_count;
        return iterator(pos.node);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::erase(const_iterator first, const_iterator last) -> iterator {
        if (first != last) {
            if (first == begin() && last == end()) {
                clear();
            } else {
                size_type bkt = BKT_NUM(*first);
                bucket_type cur = find_node_before(bkt, static_cast<bucket_type>(first.node));

                while (first != last) {
                    destroy_node(static_cast<bucket_type>((first++).node));
                    --_count;
                }

                _buckets[bkt] = static_cast<bucket_type>(last.node);
                if (cur) {
                    cur -> next = last.node;
                } else {
                    _start = _buckets[bkt];
                }
            }
        }

        return iterator(last.node);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::erase(const V& value) -> size_type {
        size_type old_count = _count;
        HxSTL::pair<const_iterator, const_iterator> pr = equal_range(value);
        erase(pr.first, pr.second);
        return old_count - _count;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::rehash(size_type count) {
        if (count < size() / max_load_factor()) {
            // 暂不考虑异常
            bucket_type first = _start;

            _bucket_alloc.deallocate(_buckets, _bucket_count);

            initialize_aux(_bucket_count);

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
