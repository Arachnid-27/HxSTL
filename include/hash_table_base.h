#ifndef _HASH_TABLE_BASE_H_
#define _HASH_TABLE_BASE_H_


#include "iterator.h"


namespace HxSTL {

    struct __prime_hash_policy {
        enum {PRIME_NUM = 28};

        static constexpr size_t prime_list[PRIME_NUM] = {
            53,         97,         193,        389,
            768,        1543,       3079,       6151,
            12289,      24593,      49157,      98317,
            196613,     393241,     786433,     1572869,
            3145739,    6291469,    12582917,   25165843,
            50331653,   100663319,  201326611,  402653189,
            8805306457, 1610612741, 3221225473, 4294967291
        };

        static size_t next_buckets(size_t n) {
            for (int i = 0; i != PRIME_NUM; ++i) {
                if (prime_list[i] >= n) {
                    return prime_list[i];
                }
            }
            return prime_list[PRIME_NUM - 1];
        }

        static size_t max_buckets() {
            return prime_list[PRIME_NUM - 1];
        }
    };

    constexpr size_t __prime_hash_policy::prime_list[PRIME_NUM];

    struct __hash_node_base {
        typedef HxSTL::forward_iterator_tag     iterator_category;
        typedef __hash_node_base*               base_link_type;
        base_link_type next;
    };

    struct __hash_table_iterator_base {
        typedef __hash_node_base::base_link_type        base_link_type;

        base_link_type node;

        __hash_table_iterator_base() {}

        __hash_table_iterator_base(base_link_type x): node(x) {}
    };

    struct __hash_table_local_iterator_base: public __hash_table_iterator_base {
        size_t bucket;
        size_t bucket_count;

        __hash_table_local_iterator_base(size_t bkt, size_t cnt)
            : bucket(bkt), bucket_count(cnt) {}

        __hash_table_local_iterator_base(base_link_type x, size_t bkt, size_t cnt)
            : __hash_table_iterator_base(x), bucket(bkt), bucket_count(cnt) {}
    };

    template <class Result, class Arg>
    struct __hash_base {
        typedef Result      result_type;
        typedef Arg         argument_type;
    };

    template <class T>
    struct hash;

    template <class T>
    struct hash<T*>: public __hash_base<size_t, T*> {
        size_t operator()(T* p) const noexcept {
            return reinterpret_cast<size_t>(p);
        }
    };

#define _define_trivial_hash(T)                     \
    template <>                                     \
    struct hash<T>: public __hash_base<size_t, T> { \
        size_t operator()(T value) const noexcept { \
            return static_cast<size_t>(value);      \
        }                                           \
    };

    _define_trivial_hash(bool);

    _define_trivial_hash(char);

    _define_trivial_hash(signed char);

    _define_trivial_hash(unsigned char);

    _define_trivial_hash(wchar_t);

    _define_trivial_hash(char16_t);

    _define_trivial_hash(char32_t);

    _define_trivial_hash(short);

    _define_trivial_hash(unsigned short);

    _define_trivial_hash(int);

    _define_trivial_hash(unsigned int);

    _define_trivial_hash(long);

    _define_trivial_hash(unsigned long);

    _define_trivial_hash(long long);

    _define_trivial_hash(unsigned long long);

#undef _define_trivial_hash

}


#endif
