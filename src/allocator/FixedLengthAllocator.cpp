//
// Created by anarion on 4/14/20.
//

#include "allocator/FixedLengthAllocator.h"

anarion::FixedLengthAllocator::chunk::chunk(anarion::FixedLengthAllocator::chunk *next) : next(next) {}

void anarion::FixedLengthAllocator::makeList(anarion::FixedLengthAllocator::chunk *begin, size_t step, size_t num) {
    if (num == 0) {
        return;
    }
    chunk *p = begin;
    for (size_t i = 0; i < num - 1; ++i) {
        chunk *next = (chunk*)((char*)p + step);
        p->next = next;
//        next->next = nullptr;
        p = next;
    }
    p->next = nullptr;
}

void *anarion::FixedLengthAllocator::allocate() {
    lock.lock();
    if (head->next == nullptr) {
        head->next = allocSlot(count);
        count += count;
    }
    void *ret = head;
    head = head->next;
    lock.unlock();
    return ret;
//    return malloc(length);
}

void anarion::FixedLengthAllocator::deallocate(void *p) {
    lock.lock();
    memset(p, 0, length);
    chunk *ptr = static_cast<chunk *>(p);
    ptr->next = head;
    head = ptr;
    lock.unlock();
//    free(p);
}
