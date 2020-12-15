//
// Created by anarion on 12/15/20.
//

#include "concurrent/pool/AbstractThreadPool.h"

void anarion::AbstractThreadPool::ThreadEntry::mainRoutine() {
    while (true) {
        // wait for non running state
        acquireTaskEntrance();
        // check for different states
        if (state == isReady) {
            // normal handling
            enterBusy();
        } else if (state == isDead) {
            // entering dead state
            releaseTaskEntrance();
            break;
        } else {
            // gibberish thread state
            enterDead();
            releaseTaskEntrance();
            throw ThreadStateUnknown();
        }
        // execute task
        if (executable == nullptr) {
            throw NullExecutable();
        }
        executable->run();

        enterPending();
        releaseTaskEntrance();
    }
}

void *anarion::AbstractThreadPool::ThreadEntry::entryRoutine(void *entryPointer) {
    auto *entry = static_cast<ThreadEntry *>(entryPointer);
    entry->mainRoutine();
    return nullptr;
}

void anarion::AbstractThreadPool::ThreadEntry::monitorStartRoutine() {
    enterPending();
    thread.startPThread(entryRoutine, this);
}

void anarion::AbstractThreadPool::ThreadEntry::monitorSetExecutable(anarion::PoolExecutable *e) {
    executable = e;
}

void anarion::AbstractThreadPool::ThreadEntry::acquireTaskEntrance() {
    mutex.lock();
    while (state == isPending) {
        cond.wait();
    }
    ++refCount;
    if (refCount > 1) {
        releaseTaskEntrance();
        throw EntryUsedByMultipleMonitor();
    }
}

void anarion::AbstractThreadPool::ThreadEntry::enterBusy() {
    pool->markBusy(poolIndex, this);
    state = isBusy;
}

void anarion::AbstractThreadPool::ThreadEntry::enterPending() {
    pool->markPending(poolIndex, this);
    state = isReady;
}

void anarion::AbstractThreadPool::ThreadEntry::enterDead() {
    state = isDead;
    pool->markDead(poolIndex, this);
}

void anarion::AbstractThreadPool::ThreadEntry::releaseTaskEntrance() {
    --refCount;
    mutex.unlock();
}

bool anarion::AbstractThreadPool::ThreadEntry::monitorLaunchTask() {
    if (state != isPending) {
        return false;
    }
    enterReady();
    cond.broadcast();
    return true;
}

bool anarion::AbstractThreadPool::ThreadEntry::monitorKillUnBusyRoutine() {
    if (state != isPending) {
        return false;
    }
    enterDead();
    cond.broadcast();
    return true;
}

void anarion::AbstractThreadPool::ThreadEntry::enterReady() {
    state = isReady;
    pool->markReady(poolIndex, this);
}

void anarion::AbstractThreadPool::startMainRoutine(size_type index, anarion::AbstractThreadPool::ThreadEntry *entry) {
    // check params
    if (entry->state != ThreadEntry::isDead) {
        throw ThreadAlreadyStarted();
    }
    // setup params
    entry->poolIndex = index;
    entry->pool = this;
    // call monitorStartRoutine
    entry->monitorStartRoutine();
}
