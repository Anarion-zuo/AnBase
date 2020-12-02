//
// Created by anarion on 12/2/20.
//

#ifndef ANBASE_SIMPLEALLOCATOR_H
#define ANBASE_SIMPLEALLOCATOR_H

#include "Allocator.h"

namespace anarion {
class SimpleAllocator : public Allocator {
public:
    void *allocate(size_t size) override;

    void deallocate(void *p, size_t num) override;
};
}

#endif //ANBASE_SIMPLEALLOCATOR_H
