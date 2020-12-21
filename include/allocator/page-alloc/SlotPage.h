//
// Created by anarion on 8/11/20.
//

#ifndef ANBASE_SLOTPAGE_H
#define ANBASE_SLOTPAGE_H

#include <container/List/BitArray.h>
#include "FreeSlotInfo.h"

namespace anarion {
namespace page_alloc {
    using size_type = unsigned long;
    class SlotPage {
    protected:
        size_type slotSize, slotCount;
        FreeSlotInfo *headPointer = nullptr;
        BitArray usedSlots;

        size_type ptrIndex(FreeSlotInfo *ptr) const ;
    public:
        SlotPage(size_type slotSize, size_type slotCount);
        ~SlotPage();

        constexpr size_type getSlotCount() const { return slotCount; }
        constexpr size_type getSlotSize() const { return slotSize; }
        constexpr size_type getPageSize() const { return slotCount * slotSize; }

        void markUsed(FreeSlotInfo *ptr);
        void markedUnused(FreeSlotInfo *ptr);
        FreeSlotInfo *getPage(size_type index) const ;
        FreeSlotInfo *getLastPage() const ;
        FreeSlotInfo *getFristPage() const ;

        bool isUsed() const ;
    };
}
}

#endif //ANBASE_SLOTPAGE_H
