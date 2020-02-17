//
// Created by 左钰 on 2020/1/14.
//

#ifndef MYCPPLIB_LISTALLOCATOR_H
#define MYCPPLIB_LISTALLOCATOR_H

void initListAllocator();
void execListAllocatorAllocate(char **p, unsigned long num);
void execListAllocatorDeallocate(char *p, unsigned long num);

#endif //MYCPPLIB_LISTALLOCATOR_H
