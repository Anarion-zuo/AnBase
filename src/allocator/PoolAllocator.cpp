//
// Created by anarion on 2020/1/29.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#include "allocator/PoolAllocator.h"
#include <malloc.h>
#include <cstring>

#define InitPoolSize 256

typedef PoolAllocator::size_type size_type;

struct chunk {
    chunk *next;
};

static chunk *pool_head, *pool_end;
static size_type allocated_size = 0;
static chunk pool_list_heads[16];

static void reallocate_pool(size_type);

static void init() {
    reallocate_pool(InitPoolSize);
    memset(pool_list_heads, 0, sizeof(chunk) * 16);
    allocated_size = InitPoolSize;
}

static bool empty_pool() {
    return pool_end == pool_head;
}

static size_type pool_size() {
    return (char*)pool_end - (char*)pool_head;
}

static void reallocate_pool(size_type num) {
    char *ptr = (char*)::malloc(num);
    pool_head = reinterpret_cast<chunk *>(ptr);
    pool_end = reinterpret_cast<chunk*>(ptr + num);
}

static size_type index2size(size_type index) {
    return (index + 1) << 3u;
}

static size_type size2index(size_type size) {
    size_type index = size >> 3u;
    size_type rec = index << 3u;
    if (rec == size) {
        return index - 1;
    }
    return index;
}

static size_type index_max_num(size_type index, size_type num) {
    return num >> (index + 3);
}

static chunk *make_list(chunk *begin, size_type step, size_type num) {
    chunk *cur = begin;
    char *next;
    --num;   // special handling for the last
    for (size_type i = 0; i < num; ++i) {
        next = (char *)cur + step;
        cur->next = (chunk*)next;
        cur = (chunk*)next;
    }
    cur->next = nullptr;
    return cur;
}

static void expand_pool() {            // expand when empty
    size_type newsize = allocated_size << 1u;
    reallocate_pool(newsize);
    allocated_size += newsize;
}

// append some space to certain list
// assume pool is sufficient for the request
static void list_append_pool(size_type index, size_type num) {
    chunk *p = pool_head;
    char *_head = (char*)pool_head;
    _head += num << (index + 3u);
    pool_head = (chunk *)_head;
    chunk *last = make_list(p, 1u << (index + 3), num);
    last->next = pool_list_heads[index].next;
    pool_list_heads[index].next = p;
}

static void fetch2index(size_type index) {
    size_type request_size = index2size(index),
    cur_pool_size = pool_size(),
    norm_size = request_size << 4u;    // normally give 16 with extra
    if (norm_size > cur_pool_size) {
        if (cur_pool_size == 0) {
            // empty pool
            expand_pool();
        } else {
            // pool too small
            // see if can get some
            size_type maxnum = index_max_num(index, cur_pool_size);
            bool flag = false;
            if (maxnum > 0) {
                // can get some
                list_append_pool(index, maxnum);
                // got some left
                cur_pool_size = pool_size();
                flag = true;  // already appended, request fulfilled
            }
            // append what's left to best fit list
            size_type leftindex = size2index(cur_pool_size);
            list_append_pool(leftindex, 1);
            // now the pool is empty
            if (flag) {
                // request fulfilled
                return ;  // deal with empty pool next time
            }
            expand_pool();   // request not fulfilled
        }
    }
    // fulfilled request
    list_append_pool(index, 16);
}

static chunk *get_one(size_type index) {
    chunk *list_head = &pool_list_heads[index];
    if (list_head->next == nullptr) {
        // empty list
        fetch2index(index);
    }
    // not empty
    chunk *p = list_head->next;
    list_head->next = p->next;
    return p;
}

static void put_one_back(chunk *p, size_type size) {
    size_type index = size2index(size);
    chunk *next = pool_list_heads[index].next;
    pool_list_heads[index].next = p;
    p->next = next;
}

void PoolAllocator::init() {
    ::init();
}

char *PoolAllocator::allocate(size_type byteNum) {
    if (byteNum == 0) {
        return nullptr;
    }
    if (byteNum > 128) {
        return (char*)::malloc(byteNum);
    }
    return (char*)get_one(size2index(byteNum));
}

void PoolAllocator::deallocate(char *p, size_type byteNum) {
    if (p == nullptr) {
        return;
    }
    if (byteNum > 128) {
        free(p);
        return;
    }
    put_one_back(reinterpret_cast<chunk *>(p), byteNum);
}

void PoolAllocator::print(char *p) {
    chunk *node = (chunk*)p;
    size_type i = 0;
    while (node) {
        printf("%p %lu\n", node, i);
        node = node->next;
        ++i;
    }
    printf("\n");
}
