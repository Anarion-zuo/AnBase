//
// Created by 左钰 on 2020/1/10.
//

#ifndef MYCPPLIB_ALLOCATOR_H
#define MYCPPLIB_ALLOCATOR_H


#include <cstdlib>
#include <new>

/*
 * underlying allocator needs only provide interface with:
 * input as numbers of bytes
 * output as pointer to the first byte
 */

class Allocator {
public:
    virtual void *allocate(size_t size) = 0;
    virtual void deallocate(void *p, size_t num) = 0;
};



#endif //MYCPPLIB_ALLOCATOR_H
