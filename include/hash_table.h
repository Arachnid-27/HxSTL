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
        typedef size_t                          size_type;
        typedef ptrdiff_t                       difference_type;
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
        HxSTL::pair<bool, bucket_type> find_node_before(size_type bkt, const Value& value);
        void initialize_aux(size_type count);
        void copy_aux(bucket_type* buckets);
        void insert_aux(size_type bkt, bucket_type node);
        size_type erase_aux(const Value& value);
        void clear_aux();
    public:
        hash_table(size_type bucket, const Hash& hash, const Equal& equal, const Alloc& alloc)
            : _max_factor(1.0), _count(0), _hash(hash), _equal(equal), _alloc(alloc) {
                initialize_aux(bucket);
            }

        hash_table(const hash_table& other): _max_factor(other._max_factor), _count(other._count),
            _buckets(other._buckets), _bucket_count(other._bucket_count), _start(other._start), 
            _hash(other._hash), _equal(other._equal), _alloc(other._alloc) {
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

        hash_table& operator=(const hash_table& other);

        hash_table&& operator=(hash_table&& other) {
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

        HxSTL::pair<iterator, iterator> equal_range(const Key& key);

        HxSTL::pair<const_iterator, const_iterator> equal_range(const Key& key) const;

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
        _start = nullptr;
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::copy_aux(bucket_type* buckets) {
        bucket_type cur, prev;
        for (size_type i = 0; i != _bucket_count; ++i) {
            cur = buckets[i];
            if (cur) {
                _buckets[i] = create_node(cur -> value);
                prev = _buckets[i];
                cur = cur -> next;
                while (cur) {
                    bucket_type node = create_node(cur -> value);
                    prev -> next = node;
                    prev = node;
                    cur = cur -> next;
                }
                prev -> next = cur;
            }
        }
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
    template <class T>
    auto hash_table<K, V, Ex, Eq, H, A>::insert_unique(T&& value) -> HxSTL::pair<iterator, bool> {
        rehash(_count + 1);

        size_type bkt = BKT_NUM(value);

        for (bucket_type cur = _buckets[bkt]; cur && BKT_NUM(cur) == bkt; cur = NEXT(cur)) {
            if (EQUAL(value, cur -> value)) {
                return HxSTL::pair<iterator, bool>(cur, false);
            }
        }
        
        bucket_type node = create_node(HxSTL::forward<T>(value));
        insert_aux(bkt, node);
        ++_count;
        return HxSTL::pair<iterator, bool>(node, true);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::equal_range(const K& key) -> HxSTL::pair<iterator, iterator> {
        iterator it1 = find(key);
        iterator it2 = it1;
        iterator first = it1;
        iterator last = end(bucket(key));
        while (it2 != last) {
            if (Eq()(key, Ex()(*first))) {
                it2 = first;
            }
            ++it2;
        }
        return HxSTL::pair<iterator, iterator>(it1, it2);
    }

    template <class K, class V, class Ex, class Eq, class H, class A>
    auto hash_table<K, V, Ex, Eq, H, A>::equal_range(const K& key) const -> HxSTL::pair<const_iterator, const_iterator> {
        const_iterator it1 = find(key);
        const_iterator it2 = it1;
        const_iterator first = it1;
        const_iterator last = end(bucket(key));
        while (it2 != last) {
            if (Eq()(key, Ex()(*first))) {
                it2 = first;
            }
            ++it2;
        }
        return HxSTL::pair<const_iterator, const_iterator>(it1, it2);
    }
    template <class K, class V, class Ex, class Eq, class H, class A>
    void hash_table<K, V, Ex, Eq, H, A>::rehash(size_type count) {
        if (count < size() / max_load_factor()) {
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
