//
// Created by 左钰 on 2020/1/10.
//

#include "Allocator.h"

char *Allocator::allocate(size_t size) {
    char *p;
    execListAllocatorAllocate(&p, size);
    return p;
}

void Allocator::deallocate(char *p, size_t num) {
    execListAllocatorDeallocate(p, num);
}
