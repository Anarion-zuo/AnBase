//
// Created by anarion on 5/9/20.
//

#ifndef ANBASE_ALLOCATOR_EXCEPTIONS_H
#define ANBASE_ALLOCATOR_EXCEPTIONS_H

#include <exception>

namespace anarion {
struct MallocOutOfMemory : std::exception {
    const char * what() const noexcept override {
        return "Malloc run out of memory";
    }
};
}
#endif //ANBASE_ALLOCATOR_EXCEPTIONS_H
