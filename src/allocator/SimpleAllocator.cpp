//
// Created by anarion on 12/2/20.
//

#include "allocator/SimpleAllocator.h"

void *anarion::SimpleAllocator::allocate(size_t size) {
    return operator new(size);
}

void anarion::SimpleAllocator::deallocate(void *p, size_t num) {
    operator delete(p, num);
}
