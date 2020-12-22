//
// Created by anarion on 12/15/20.
//

#include "concurrent/pool/AbstractThreadPool.h"

void anarion::AbstractThreadPool::ThreadEntry::mainRoutine() {
    while (true) {
        // wait for non running state
        acquireTaskEntrance();
        // check for different states
        if (state == isBusy) {
            // normal handling
//            enterBusy();
        } else if (state == isDead) {
            // entering dead state
            releaseTaskEntrance();
            break;
        } else {
            // gibberish thread state
            leaveFree();
            leaveBusy();
            enterDead();
            releaseTaskEntrance();
            throw ThreadStateUnknown();
        }
        // execute task
        if (executable == nullptr) {
            throw NullExecutable();
        }
        executable->run();

        leaveBusy();
        enterFree();
        releaseTaskEntrance();
    }
}

void *anarion::AbstractThreadPool::ThreadEntry::entryRoutine(void *entryPointer) {
    auto *entry = static_cast<ThreadEntry *>(entryPointer);
    entry->mainRoutine();
    return nullptr;
}

void anarion::AbstractThreadPool::ThreadEntry::monitorStartRoutine() {
    leaveDead();
    enterFree();
    thread.startPThread(entryRoutine, this);
}

void anarion::AbstractThreadPool::ThreadEntry::monitorSetExecutable(anarion::PoolExecutable *e) {
    executable = e;
}

void anarion::AbstractThreadPool::ThreadEntry::acquireTaskEntrance() {
    mutex.lock();
    while (state == isFree) {
        cond.wait();
    }
    ++refCount;
    if (refCount > 1) {
        releaseTaskEntrance();
        throw EntryUsedByMultipleMonitor();
    }
}

void anarion::AbstractThreadPool::ThreadEntry::enterFree() {
    pool->markFree(poolIndex, this);
    state = isFree;
}

void anarion::AbstractThreadPool::ThreadEntry::enterDead() {
    pool->markDead(poolIndex, this);
    state = isDead;
}

void anarion::AbstractThreadPool::ThreadEntry::releaseTaskEntrance() {
    --refCount;
    mutex.unlock();
    cond.broadcast();
}

bool anarion::AbstractThreadPool::ThreadEntry::monitorLaunchTask() {
    if (state != isFree) {
        return false;
    }
    leaveFree();
    enterBusy();
    cond.broadcast();
    return true;
}

bool anarion::AbstractThreadPool::ThreadEntry::monitorKillUnBusyRoutine() {
    if (state != isFree) {
        return false;
    }
    leaveFree();
    enterDead();
    cond.broadcast();
    return true;
}

void anarion::AbstractThreadPool::ThreadEntry::enterBusy() {
    state = isBusy;
    pool->markBusy(poolIndex, this);
}

void anarion::AbstractThreadPool::ThreadEntry::monitorWaitComplete() {
    if (state != isBusy) {
        return;
    }
    mutex.lock();
    while (state == isBusy) {
        cond.wait();
    }
    mutex.unlock();
}

void anarion::AbstractThreadPool::ThreadEntry::leaveFree() {
    pool->unmarkFree(poolIndex, this);
}

void anarion::AbstractThreadPool::ThreadEntry::leaveDead() {
    pool->unmarkDead(poolIndex, this);
}

void anarion::AbstractThreadPool::ThreadEntry::leaveBusy() {
    pool->unmarkBusy(poolIndex, this);
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

anarion::AbstractThreadPool::ThreadEntry &anarion::AbstractThreadPool::getEntry(anarion::size_type index) {
    return entries.get(index);
}

const anarion::AbstractThreadPool::ThreadEntry &anarion::AbstractThreadPool::getEntry(anarion::size_type index) const {
    return entries.get(index);
}

void anarion::AbstractThreadPool::start(anarion::size_type index) {
    startMainRoutine(index, &getEntry(index));
}

void anarion::AbstractThreadPool::launch(anarion::size_type index) {
    getEntry(index).monitorLaunchTask();
}

void anarion::AbstractThreadPool::setTask(anarion::size_type index, anarion::PoolExecutable *executable) {
    getEntry(index).monitorSetExecutable(executable);
}

bool anarion::AbstractThreadPool::tryKill(anarion::size_type index) {
    return getEntry(index).monitorKillUnBusyRoutine();
}

bool anarion::AbstractThreadPool::isBusy(anarion::size_type index) {
    return getEntry(index).state == ThreadEntry::isBusy;
}

bool anarion::AbstractThreadPool::isFree(anarion::size_type index) {
    return getEntry(index).state == ThreadEntry::isFree;
}

bool anarion::AbstractThreadPool::isDead(anarion::size_type index) {
    return getEntry(index).state == ThreadEntry::isDead;
}

void anarion::AbstractThreadPool::join(anarion::size_type index) {
    getEntry(index).monitorWaitComplete();
    getEntry(index).monitorKillUnBusyRoutine();
}
