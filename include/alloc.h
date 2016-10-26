#ifndef _ALLOC_H_
#define _ALLOC_H_


#include <cstdlib>
#include "allocator.h"


namespace HxSTL {

    class alloc {
    private:
        // enum hack
        enum {__ALIGN = 8};
        enum {__MAX_BYTES = 128};
        enum {__NFREELIST = __MAX_BYTES/__ALIGN};
    private:
        static size_t ROUND_UP(size_t bytes) {
            return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1)); 
        }

        static size_t FREEELIST_INDEX(size_t bytes) {
            return ((bytes + __ALIGN - 1) / (__ALIGN - 1));
        }
    private:
        union obj {
            union obj *free_list_link;
            char client_data[1];
        };

        static obj *free_list[__NFREELIST];
    private:
        static char *start_free;
        static char *end_free;
        static size_t heap_size;
    private:
        static void *refill(size_t n);
        static char *chunk_alloc(size_t size, int& nobjs);
    public:
        static void *allocate(size_t n);
        static void deallocate(void* p, size_t n);
    };

}


#endif
