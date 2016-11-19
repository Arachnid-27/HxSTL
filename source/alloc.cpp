#include "alloc.h"


namespace HxSTL {

    alloc::obj *alloc::free_list[__NFREELIST] = 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    char *alloc::start_free = 0;
    char *alloc::end_free = 0;
    size_t alloc::heap_size = 0;

    void *alloc::allocate(size_t n) {
        obj **my_free_list;
        obj *result;
        
        if (n > __MAX_BYTES) {
            return malloc(n);
        }

        // 在 freelist 寻找
        my_free_list = free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        // 没找到则重新填充
        if (result == 0) {
            return refill(ROUND_UP(n));
        }

        // 调整节点
        *my_free_list = result -> free_list_link;
        return result;
    }

    void alloc::deallocate(void *p, size_t n) {
        obj **my_free_list;
        obj *q = static_cast<obj*>(p);

        if (n > __MAX_BYTES) {
            free(p);
        } else {
            my_free_list = free_list + FREELIST_INDEX(n);
            q -> free_list_link = *my_free_list;
            *my_free_list = q;
        }
    }

    // n 已经 ROUND_UP
    void *alloc::refill(size_t n) {
        // 尝试填充区块个数
        int nobjs = 20;
        char *chunk = chunk_alloc(n, nobjs);

        obj *current_obj;

        if (nobjs != 1) {
            free_list[FREELIST_INDEX(n)] = current_obj = (obj*) (chunk + n);
            for (int i = 2; i != nobjs; ++i) {
                current_obj -> free_list_link = (obj*) ((char*) current_obj + n);
                current_obj = current_obj -> free_list_link;
            }
            current_obj -> free_list_link = 0;
        }

        return chunk;
    }

    char *alloc::chunk_alloc(size_t size, int &nobjs) {
        char *result;
        size_t total_bytes = size * nobjs;
        size_t left_bytes = end_free - start_free;

        if (left_bytes >= total_bytes) {
            // 剩余容量满足需求
            result = start_free;
            start_free += total_bytes;
            return result;
        } else if (left_bytes >= size) {
            // 剩余容量足够供应部分区块 
            nobjs = left_bytes / size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        } else {
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);

            if (left_bytes > 0) {
                // left_bytes 对齐的 所以可以直接加入
                obj **my_free_list = free_list + FREELIST_INDEX(left_bytes);
                ((obj*) start_free) -> free_list_link = *my_free_list;
                *my_free_list = (obj*) start_free; 
            } 

            start_free = (char*) malloc(bytes_to_get);
            if (start_free == 0) {
                obj **my_free_list;
                obj *p;
                // 查找空闲链表中是否存在足够大的区块
                for (int i = 0; i != __MAX_BYTES; i += __ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (p != 0) {
                        // 释放区块
                        *my_free_list = p -> free_list_link;
                        start_free = (char*) p;
                        end_free = start_free + i;
                        return chunk_alloc(size, nobjs);
                    }
                }
                end_free = 0;
                return 0;
            } else {
                heap_size += bytes_to_get;
                end_free = start_free + bytes_to_get;
                return chunk_alloc(size, nobjs);
            }
        }
    }

}
