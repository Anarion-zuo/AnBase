//
// Created by 左钰 on 2020/1/21.
//

#include "allocator/DynamicListAllocator.h"
#include <cstdlib>
#include <cstring>

typedef unsigned long size_type;

/*
 * 8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 128
 */

struct chunk {
    chunk * next;
};

static chunk poolHeads[16];
static chunk *poolReserve;
static size_type poolSize;
static size_type headSizes[16];

static void initializeDynamicListAllocator() {
    // initialize each pool with 20 members
    // 20 * (8 + ... + 128)
    poolReserve = (chunk*)malloc(20);
    memset(poolHeads, 0, 16 * sizeof(chunk));
    memset(headSizes, 0, sizeof(size_type) * 16);
    poolSize = 20;
}

size_type index2Size(size_type index) {
    return (index + 1) << 3u;
}

size_type size2Index(size_type size) {
    size_type m = size >> 3u, mm = m << 3u;
    if (mm < size) {
        return m + 1;
    }
    return m;
}

static void aqcuireMore(size_type index) {

}

static chunk *getOne(size_type index) {
    chunk *p = &poolHeads[index];
    if (p->next == nullptr) {

    }
}
