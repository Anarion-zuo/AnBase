//
// Created by anarion on 4/14/20.
//

#ifndef MYCPPBASE_FIXEDLENGTHALLOCATOR_H
#define MYCPPBASE_FIXEDLENGTHALLOCATOR_H


#include <cstdlib>
#include <cstring>

namespace anarion {
    class FixedLengthAllocator {
        struct chunk {
            chunk *next;

            explicit chunk(chunk *next = nullptr);
        };

        size_t length;

        static const unsigned long ptr_len = sizeof(void*);

        chunk *head;
        size_t count = 1;

        chunk *allocSlot(size_t n) {
            chunk *ret = static_cast<chunk *>(malloc(n * length));
            makeList(ret, length, n);
            return ret;
        }

        static void makeList(chunk *begin, size_t step, size_t num);

    public:
        explicit FixedLengthAllocator(size_t length) : length(length) {
            if (length < ptr_len) { this->length = ptr_len; }
            head = static_cast<chunk *>(malloc(length));
            head->next = nullptr;
        }

        void *allocate();
        void deallocate(void *p);
        constexpr size_t getLength() const { return length; }

//        void reset();
    };
}

#endif //MYCPPBASE_FIXEDLENGTHALLOCATOR_H
