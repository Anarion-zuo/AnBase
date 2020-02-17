//
// Created by anarion on 2020/1/29.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_POOLALLOCATOR_H
#define MYCPPLIB_POOLALLOCATOR_H


class PoolAllocator {
public:
    typedef unsigned long size_type;
    static void init();
    static char *allocate(size_type byteNum);
    static void deallocate(char *p, size_type byteNum);
    static void print(char *p);
};


#endif //MYCPPLIB_POOLALLOCATOR_H
