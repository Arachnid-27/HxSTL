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

    struct __hash_node_base {
        typedef __hash_node_base*      base_link_type;
        base_link_type next;
    };

    struct __hash_table_iterator_base {
        typedef __hash_node_base::base_link_type        base_link_type;

        base_link_type node;

        __hash_table_iterator_base() {}

        __hash_table_iterator_base(base_link_type x): node(x) {}

        void increment() noexcept {
            node = node -> next;
        }
    };

}


#endif
