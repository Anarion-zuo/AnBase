//
// Created by anarion on 8/11/20.
//

#ifndef ANBASE_PAGEALLOCATOR_H
#define ANBASE_PAGEALLOCATOR_H

#include "SlotPage.h"
#include <container/Map/BplusTreeMap.hpp>

namespace anarion {
    template <>
    struct EntryGetKeyCaller<page_alloc::SlotPage*, char *> {
        char *get(const page_alloc::SlotPage *obj) { return reinterpret_cast<char *>(obj->getFristPage()); }
    };
namespace page_alloc {
    /*
     * This is where the main algorithm is implemented for the allocator I am trying to build.
     *
     * # idea
     * The allocator only handles little trunks of memory, namely, the multiples of 8:
     *      8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128
     * which are multiples from 1 to 16.
     * If the requested size is not one of these numbers, it would be rounded up to one of them.
     * If the requested size is even larger than 128, it would not be handled by the allocator. The allocator passes the request to the conventional function malloc.
     *
     * # structure
     * The allocator operates on pages. A page is a set of slots. A slot has a size of one of the multiples of 8.
     * Slots are aggregated by linked lists. Each multiple of 8 corresponds to a linked list. User program obtains a slot by reading off the head of the linked list.
     * When the slots of some multiple runs out, the allocator tries to allocate more pages holding that size of slots.
     *
     * # allocation
     *  - Round up size to a multiple of 8.
     *  - Read out the head of the linked list.
     *  - Update linked list head.
     *
     * # deallocation
     *  - Round up size to a multiple of 8.
     *  - Put the slot at the head of the linked list
     *
     * # cleaning
     *  - Check the bit map of each page.
     *  - Deallocate the all-0 ones.
     *
     * # initialization
     *  - Allocate upto 4096 Bytes for each multiple of 8, 64KB in total.
     *  - Set up member attributes.
     *
     */
    class PageAllocator {
    protected:
        // using as little types as possible
        using page_index_type = unsigned char;          // 0-15
        using handling_size_type = unsigned char;       // 8-128

        const static size_type pageSize = 4096;

        // pages
        BplusTreeEmbededMap<SlotPage*, char *> allocatedPagesMap;

        // space list head
        FreeSlotInfo *slotHeads[16];

        // init
        void allocateNewPage(page_index_type pageIndex);

        static page_index_type roundToPageIndex(handling_size_type inputSize);
        static page_index_type roundSizeToMultiple(handling_size_type inputSize);
        static handling_size_type pageIndexToSlotSize(page_index_type listIndex);

        SlotPage *getPageByAddress(FreeSlotInfo *slotInfo) ;
        FreeSlotInfo *fetchASlot(page_index_type pageIndex);
        void returnASlot(page_index_type pageIndex, FreeSlotInfo *slot);

    public:
        PageAllocator();

        char *allocate(size_type nbytes);
        void deallocate(char *ptr, size_type nbytes);
    };
    class PageAllocatorException : public std::exception {};
    class DanglingPointer : public PageAllocatorException {};
    class AllocationFailed : public PageAllocatorException {};
}
}

#endif //ANBASE_PAGEALLOCATOR_H
