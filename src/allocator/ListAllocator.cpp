//
// Created by 左钰 on 2020/1/14.
//

#include <cstdlib>
#include "ListAllocator.h"
#include "exceptions/allocator/AllocationException.h"

struct chunk {
    chunk *next;
};

typedef unsigned long size_type;
typedef unsigned int index_type;

// assuming the pointer size is 8 bytes
static index_type baseSizes[] = {8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128};    // 16 in total
// lists
static chunk *poolHeads[16], *poolEnds[16];
static size_type poolSizes[16];   // number of allocated blocks from system

static size_type index2Size(index_type index) {
    return (index + 1) << 3u;
}

static index_type size2Index(size_type size) {
    index_type index = size >> 3u;
    size_type rec = index << 3u;
    if (rec == size) {
        return index - 1;
    }
    return index;
}

static chunk *makeList(chunk *begin, size_type num, size_type step) {
    char *p = (char *)begin;
    for (size_type i = 0; i < num; ++i) {
        char *next = p + step;
        chunk *cur = (chunk*)p;
        cur->next = (chunk*)next;
        p = next;
    }
    p -= step;
    chunk *last = (chunk *)p;
    last->next = nullptr;
    return last;
}

static bool isIndexGivenOut(index_type index) {
    return poolEnds[index] == poolHeads[index];
}

static chunk *sysallocate(size_type size) {
    chunk *p = static_cast<chunk *>(::malloc(size));
    if (p == nullptr) {
        throw AllocationException();
    }
    return p;
}

static void sysdeallocate(chunk *p) {
    ::free(p);
}

static void expandIndexPool(index_type index) {
    size_type newsize = poolSizes[index];
    newsize <<= 1u;
    chunk *p = sysallocate(newsize << 3u);
    poolEnds[index]->next = p;
    poolEnds[index] = makeList(p, newsize, 8);
    poolSizes[index] += newsize;
}

static chunk *getOneBlock(index_type index) {
    if (isIndexGivenOut(index)) {
        expandIndexPool(index);
    }
    chunk *p = poolHeads[index];
    poolHeads[index] = poolHeads[index]->next;
    return p;
}

static void putOneBlockBack(chunk *p, index_type index) {
    poolEnds[index]->next = p;
    poolEnds[index] = p;
    p->next = nullptr;
}

void initListAllocator() {
    size_type step = 8;
    for (index_type i = 0; i < 16; ++i) {
        size_type num = 16, tolsize = num * step;
        chunk *p = sysallocate(tolsize);
        poolHeads[i] = p;
        poolEnds[i] = makeList(p, num, step);
        poolSizes[i] = tolsize;
        step += 8;
    }
}

void execListAllocatorAllocate(char **p, size_type num) {
    if (p == nullptr) {
        return;
    }
    if (num == 0) {
        *p = nullptr;
        return;
    }
    if (num > 128) {
        // provide no service to request too large
        *p = reinterpret_cast<char *>(sysallocate(num));
        return;
    }
    index_type index = size2Index(num);
    *p = reinterpret_cast<char *>(getOneBlock(index));
}

void execListAllocatorDeallocate(char *p, size_type num) {
    if (p == nullptr || num == 0) {
        return;
    }
    if (num > 128) {
        sysdeallocate(reinterpret_cast<chunk *>(p));
        return;
    }
    index_type index = size2Index(num);
    putOneBlockBack(reinterpret_cast<chunk *>(p), index);
}
