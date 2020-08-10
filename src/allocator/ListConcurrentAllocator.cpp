//
// Created by anarion on 2020/3/19.
//

#include <cstring>
#include "allocator/ListConcurrentAllocator.h"

using namespace anarion;

// make no assumption on pointer size
#define CHUNK_SIZE sizeof(void*)

typedef unsigned long size_type;

struct chunk {
    struct chunk * next;
    chunk(chunk *next = nullptr) : next(next) {}
};

static chunk *pool_heads[16];
static size_type pool_counts[16];

static size_type index2size(size_type index) {
    return CHUNK_SIZE * (index + 1);
}

static size_type size2index(size_type size) {
    size_type m = size / CHUNK_SIZE, n = size % CHUNK_SIZE;
    if (n == 0) { --m; }
    return m;
}

static chunk *makelist(chunk *begin, size_type step, size_type num) {
    for (size_type index = 0; index < num; ++index) {
        chunk * next = (chunk*)((char*)begin + step);
        begin->next = next;
        begin = next;
    }
    chunk *last = (chunk*)((char*)begin - step);
    last->next = nullptr;
    return last;
}

static inline void init() {
    memset(pool_heads, 0, CHUNK_SIZE * 16);
    memset(pool_counts, 0, sizeof(size_type) * 16);
    for (size_type index = 0; index < 16; ++index) {
        pool_heads[index] = static_cast<chunk *>(malloc(index2size(index) * 20));
        makelist(pool_heads[index], index2size(index), 20);
        pool_counts[index] = 20;
    }
}

static inline void tryexpand(size_type index) {
    if (pool_heads[index]->next != nullptr) {
        return;
    }
    size_type newsize = pool_counts[index] * 2;
    chunk *newp = static_cast<chunk *>(malloc(newsize * index2size(index)));
    pool_heads[index]->next = newp;
    makelist(newp, index2size(index), newsize);
}

static inline chunk *get_one(size_type index) {
    tryexpand(index);
    chunk *ret = pool_heads[index];
    pool_heads[index] = pool_heads[index]->next;
    return ret;
}

static inline void put_back(chunk *p, size_type index) {
    chunk *old_head = pool_heads[index];
    p->next = old_head;
    pool_heads[index] = p;
}

static inline chunk *allocate(size_type size) {
    if (size > 128) { return static_cast<chunk *>(malloc(size)); }
    return get_one(size2index(size));
}

static inline void deallocate(chunk *p, size_type size) {
    if (size > 128) { free(p); return; }
    put_back(p, size2index(size));
}

anarion::ListConcurrentAllocator *anarion::ListConcurrentAllocator::instance = new ListConcurrentAllocator();

anarion::ListConcurrentAllocator::ListConcurrentAllocator() {
    init();
}

void *anarion::ListConcurrentAllocator::allocate(size_t size) {
    if (size == 0) { return nullptr; }
    return ::allocate(size);
}

void anarion::ListConcurrentAllocator::deallocate(void *p, size_t num) {
    if (p == nullptr || num == 0) { return; }
    ::deallocate(static_cast<chunk *>(p), num);
}

void *ListConcurrentAllocator::operator new(size_t nbytes) {
    return ::allocate(nbytes);
}

void ListConcurrentAllocator::operator delete(void *p, size_t nbytes) {
    ::deallocate(static_cast<chunk *>(p), nbytes);
}

