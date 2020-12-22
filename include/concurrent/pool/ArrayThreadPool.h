//
// Created by anarion on 12/21/20.
//

#ifndef ANBASE_ARRAYTHREADPOOL_H
#define ANBASE_ARRAYTHREADPOOL_H

#include <container/numtype/NumListSet.h>
#include "AbstractThreadPool.h"

namespace anarion {
class ArrayThreadPool : public AbstractThreadPool {
protected:
    NumListSet<entry_t> busySet, freeSet;

    void markBusy(size_type index, ThreadEntry *entry) override;
    void markFree(size_type index, ThreadEntry *entry) override;
    void markDead(size_type index, ThreadEntry *entry) override;

    void unmarkBusy(size_type index, ThreadEntry *entry) override;
    void unmarkFree(size_type index, ThreadEntry *entry) override;
    void unmarkDead(size_type index, ThreadEntry *entry) override;

public:
    explicit ArrayThreadPool(entry_t size) : AbstractThreadPool(size), busySet(size), freeSet(size) {}
};
}

#endif //ANBASE_ARRAYTHREADPOOL_H
