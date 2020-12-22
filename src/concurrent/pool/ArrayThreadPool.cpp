//
// Created by anarion on 12/21/20.
//

#include "concurrent/pool/ArrayThreadPool.h"

void anarion::ArrayThreadPool::markBusy(anarion::size_type index, anarion::AbstractThreadPool::ThreadEntry *entry) {
    busySet.pushBack(index);
}

void anarion::ArrayThreadPool::markFree(anarion::size_type index, anarion::AbstractThreadPool::ThreadEntry *entry) {
    freeSet.pushBack(index);
}

void anarion::ArrayThreadPool::markDead(anarion::size_type index, anarion::AbstractThreadPool::ThreadEntry *entry) {

}

void anarion::ArrayThreadPool::unmarkBusy(anarion::size_type index, anarion::AbstractThreadPool::ThreadEntry *entry) {
    busySet.remove(index);
}

void anarion::ArrayThreadPool::unmarkFree(anarion::size_type index, anarion::AbstractThreadPool::ThreadEntry *entry) {
    freeSet.remove(index);
}

void anarion::ArrayThreadPool::unmarkDead(anarion::size_type index, anarion::AbstractThreadPool::ThreadEntry *entry) {

}
