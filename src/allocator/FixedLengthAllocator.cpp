//
// Created by anarion on 4/14/20.
//

#include "allocator/FixedLengthAllocator.h"

anarion::FixedLengthAllocator::chunk::chunk(anarion::FixedLengthAllocator::chunk *next) : next(next) {}

void anarion::FixedLengthAllocator::makeList(anarion::FixedLengthAllocator::chunk *begin, size_t step, size_t num) {
    chunk *p = begin;
    for (size_t i = 0; i < num; ++i) {
        chunk *next = p + step;
        p->next = next;
        next->next = nullptr;
        p = next;
    }
}

void *anarion::FixedLengthAllocator::allocate() {
    if (head->next == nullptr) {
        head->next = allocSlot(count);
        count += count;
    }
    void *ret = head;
    head = head->next;
    return ret;
}

void anarion::FixedLengthAllocator::deallocate(void *p) {
    chunk *ptr = static_cast<chunk *>(p);
    ptr->next = head;
    head = ptr;
}
