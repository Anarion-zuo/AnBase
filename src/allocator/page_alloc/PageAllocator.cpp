//
// Created by anarion on 8/11/20.
//

#include "allocator/page-alloc/PageAllocator.h"
#include <cstdlib>

anarion::page_alloc::PageAllocator::page_index_type
anarion::page_alloc::PageAllocator::roundSizeToMultiple(handling_size_type inputSize) {
    page_index_type result = inputSize / 8, mod = inputSize % 8;
    if (mod) { ++result; }
    return result;
}

anarion::page_alloc::PageAllocator::page_index_type
anarion::page_alloc::PageAllocator::roundToPageIndex(handling_size_type inputSize) {
    page_index_type result = inputSize / 8;
    return result;
}

anarion::page_alloc::PageAllocator::PageAllocator() {
    memset(slotHeads, 0, 16 * sizeof(FreeSlotInfo *));

    // init pages
    for (page_index_type index = 0; index < 16; ++index) {
        allocateNewPage(index);
    }
}

anarion::page_alloc::PageAllocator::handling_size_type
anarion::page_alloc::PageAllocator::pageIndexToSlotSize(anarion::page_alloc::PageAllocator::page_index_type listIndex) {
    return (listIndex + 1) * 8;
}

void
anarion::page_alloc::PageAllocator::allocateNewPage(anarion::page_alloc::PageAllocator::page_index_type pageIndex) {
    // create new page
    handling_size_type slotSize = pageIndexToSlotSize(pageIndex), slotCount = pageSize / slotSize;
    SlotPage *slotPage = new SlotPage(slotSize, slotCount);
    // insert into page map
    allocatedPagesMap.insert(slotPage);
    // update free list
    slotPage->getLastPage()->next = slotHeads[pageIndex];
    slotHeads[pageIndex] = slotPage->getFristPage();
}

anarion::page_alloc::FreeSlotInfo *
anarion::page_alloc::PageAllocator::fetchASlot(anarion::page_alloc::PageAllocator::page_index_type pageIndex) {
    FreeSlotInfo *result = slotHeads[pageIndex];
    if (result == nullptr) {
        allocateNewPage(pageIndex);
        result = slotHeads[pageIndex];
    }
    slotHeads[pageIndex] = slotHeads[pageIndex]->next;
    SlotPage *slotPage = getPageByAddress(result);
    if (slotPage == nullptr) {
        throw DanglingPointer();
    }
    slotPage->markUsed(result);
    return result;
}

void anarion::page_alloc::PageAllocator::returnASlot
(anarion::page_alloc::PageAllocator::page_index_type pageIndex, anarion::page_alloc::FreeSlotInfo *slot) {
    slot->next = slotHeads[pageIndex];
    slotHeads[pageIndex] = slot;
    SlotPage *slotPage = getPageByAddress(slot);
    if (slotPage == nullptr) {
        throw DanglingPointer();
    }
    slotPage->markedUnused(slot);
}

anarion::page_alloc::SlotPage *
anarion::page_alloc::PageAllocator::getPageByAddress(anarion::page_alloc::FreeSlotInfo *slotInfo) {
    auto it = allocatedPagesMap.find(reinterpret_cast<char *>(slotInfo));
    if (it == allocatedPagesMap.end_iterator()) {
        return nullptr;
    }
    if ((char *)slotInfo - (char *)(**it).getFristPage() >= (**it).getPageSize()) {
        return nullptr;
    }
    return *it;
}

char *anarion::page_alloc::PageAllocator::allocate(anarion::size_type nbytes) {
    if (nbytes > 128) {
        return static_cast<char *>(operator new(nbytes));
    }
    if (nbytes == 0) {
        return nullptr;
    }
    return reinterpret_cast<char *>(fetchASlot(roundToPageIndex(nbytes)));
}

void anarion::page_alloc::PageAllocator::deallocate(char *ptr, anarion::size_type nbytes) {
    if (nbytes > 128) {
        operator delete (ptr, nbytes);
        return;
    }
    if (nbytes == 0 || ptr == nullptr) {
        return;
    }
    returnASlot(roundToPageIndex(nbytes), reinterpret_cast<FreeSlotInfo *>(ptr));
}
