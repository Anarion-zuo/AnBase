//
// Created by anarion on 5/11/20.
//

#include "concurrent/pool/DynamicThreadPool.h"


void anarion::DynamicThreadPool::CleanThread::task() {
    auto &idles = pool->idles;
    pool->idleLock.lock();
    if (idles.empty()) {
        pool->idleLock.unlock();
        return;
    }
    joinIdleThread(pool->idles.pop());
    pool->idleLock.unlock();
}

anarion::DynamicThreadPool::CleanThread::CleanThread(anarion::Time &&time, anarion::DynamicThreadPool *pool)
        : BackgroundTask(anarion::forward<Time>(time)), pool(pool) {}

anarion::DynamicThreadPool::DynamicThreadPool() : ThreadPool(1), cleanThread(Time(1, 0), nullptr) {
    cleanThread.pool = this;
//    cleanThread.start();
}
