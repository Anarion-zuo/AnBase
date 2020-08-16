//
// Created by anarion on 8/11/20.
//

#include "allocator/page-alloc/SlotPage.h"

anarion::page_alloc::SlotPage::SlotPage(anarion::size_type slotSize, anarion::size_type slotCount)
    : slotCount(slotCount), slotSize(slotSize), usedSlots(slotCount), headPointer(static_cast<FreeSlotInfo *>(operator new(slotSize * slotCount))) {
    memset(headPointer, 0, getPageSize());
    FreeSlotInfo::makeList(reinterpret_cast<char *>(headPointer), slotCount, slotSize);
}

anarion::page_alloc::SlotPage::~SlotPage() {
    operator delete (headPointer, getPageSize());
}

anarion::size_type anarion::page_alloc::SlotPage::ptrIndex(anarion::page_alloc::FreeSlotInfo *ptr) const {
    return ((char *)ptr - (char *)headPointer) / slotSize;
}

void anarion::page_alloc::SlotPage::markUsed(anarion::page_alloc::FreeSlotInfo *ptr) {
    size_type index = ptrIndex(ptr);
    usedSlots.set(index);
}

void anarion::page_alloc::SlotPage::markedUnused(anarion::page_alloc::FreeSlotInfo *ptr) {
    usedSlots.set(ptrIndex(ptr));
}

anarion::page_alloc::FreeSlotInfo *anarion::page_alloc::SlotPage::getPage(anarion::size_type index) const {
    return reinterpret_cast<FreeSlotInfo *>((char *) headPointer + index * slotSize);
}

bool anarion::page_alloc::SlotPage::isUsed() const {
    return usedSlots.orAll();
}

anarion::page_alloc::FreeSlotInfo *anarion::page_alloc::SlotPage::getLastPage() const {
    return getPage(slotCount - 1);
}

anarion::page_alloc::FreeSlotInfo *anarion::page_alloc::SlotPage::getFristPage() const {
    return getPage(0);
}
