//
// Created by 左钰 on 2020/1/10.
//

#ifndef MYCPPLIB_ALLOCATOR_H
#define MYCPPLIB_ALLOCATOR_H


#include <cstdlib>
#include "ListAllocator.h"


/*
 * underlying allocator needs only provide interface with:
 * input as number of bytes
 * output as pointer to the first byte
 */

class Allocator {
public:
    static char *allocate(size_t size);
    static void deallocate(char *p, size_t num);
};

template <typename T> T *newSpace() {
    return (T*)malloc(sizeof(T));
}

template <typename T> T *newSpaces(size_t num) {
    return (T*)malloc(num * sizeof(T));
}

template <typename T> void delSpace(T *p) {
    free(p);
}

template <typename T> void delSpaces(T *p, size_t num) {
    free(p);
}

template <typename T> void clearSpaces(T *p, size_t num) {
    for (size_t i = 0; i < num; ++i) {
        p[i].~T();
    }
}

template <typename T, typename ...Args> T *newObject(Args ...args) {
    // allocate from memory
    T *p = newSpace<T>();
//    T *p = (T*)Allocator::allocate(sizeof(T));
    // run constructor
    ::new(p) T(args...);
    return p;
}

template <typename T> T *newObject() {
    // allocate from memory
    T *p = newSpace<T>();
//    T *p = (T*)Allocator::allocate(sizeof(T));

    // run constructor
    ::new(p) T();
    return p;
}

template <typename T> void delObject(T *p) {
    // run destructor
    clearSpaces(p, 1);

    // deallocate
    delSpace(p);
//    Allocator::deallocate((char*)p, sizeof(T));
}

template <typename T> T *newObjects(size_t num) {
    // allocate from memory
    T *p = newSpaces<T>(num);
//    T *p = (T*)Allocator::allocate(sizeof(T) * num);

    // run constructors
    for (size_t i = 0; i < num; ++i) {
        ::new(&p[i]) T();
    }
    return p;
}

template <typename T, typename ...Args> T *newObjects(size_t num, Args ...args) {
    // allocate from memory
    T *p = newSpaces<T>(num);
//    T *p = (T*)Allocator::allocate(sizeof(T) * num);

    // run constructors
    for (size_t i = 0; i < num; ++i) {
        ::new(&p[i]) T(args...);
    }
    return p;
}

template <typename T> static void delObjects(T *p, size_t num) {
    // run destructors
    clearSpaces(p, num);

    // deallocate
    delSpaces(p, num);
//    Allocator::deallocate((char*)p, sizeof(T) * num);
}
//
//void *operator new(size_t num) {
//    if (num == 0) { return nullptr; }
//    return malloc(num);
//}
//
//void operator delete(void *p, size_t num) noexcept {
//    free(p);
//}
//
//void *operator new[](size_t num) {
//    if (num == 0) { return nullptr; }
//    return malloc(num);
//}
//
//void operator delete[](void *p, size_t num) noexcept {
//    free(p);
//}


#endif //MYCPPLIB_ALLOCATOR_H
