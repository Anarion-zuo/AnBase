//
// Created by anarion on 8/11/20.
//

#include "allocator/page-alloc/FreeSlotInfo.h"

void anarion::page_alloc::FreeSlotInfo::makeList(char *begin, unsigned long counts, unsigned long step) {
    char *cur = begin;
    for (unsigned long index = 0; index < counts - 1; ++index) {
        char *next = cur + step;
        FreeSlotInfo *curInfo = reinterpret_cast<FreeSlotInfo *>(cur);
        curInfo->next = reinterpret_cast<FreeSlotInfo *>(next);
        cur = next;
    }
    ((FreeSlotInfo *)cur)->next = nullptr;
}
