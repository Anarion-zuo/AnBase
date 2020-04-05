//
// Created by anarion on 2020/3/19.
//

#ifndef MYCPPBASE_LISTCONCURRENTALLOCATOR_H
#define MYCPPBASE_LISTCONCURRENTALLOCATOR_H

#include "Allocator.h"

namespace anarion {
    class ListConcurrentAllocator : virtual public Allocator {

        ListConcurrentAllocator();
        ListConcurrentAllocator(const ListConcurrentAllocator &) = default;
        ListConcurrentAllocator(ListConcurrentAllocator &&) noexcept = default;
        ListConcurrentAllocator &operator=(const ListConcurrentAllocator &) = default;
        ListConcurrentAllocator &operator=(ListConcurrentAllocator &&) noexcept = default;

        static ListConcurrentAllocator *instance;

    public:

        static inline ListConcurrentAllocator *getInstance() { return instance; }

        void *allocate(size_t size) override;
        void deallocate(void *p, size_t num) override;

    };
}

#endif //MYCPPBASE_LISTCONCURRENTALLOCATOR_H
