//
// Created by 左钰 on 2020/1/16.
//

#include "ThreadPool.h"

using namespace anarion;

void ThreadPool::joinAll() {
    BlockQueue<info*> hold;
    for (size_type i = 0; i < count; ++i) {
        hold.put(idles.pop());
    }
    idles = std::move(hold);
}

void *ThreadPool::pool_routine(void *p) {
    auto *info = reinterpret_cast<struct info*>(p);
    while (true) {
        info->mutex.lock();   // waiting to be signalled
        info->callee->run();
        // put back idle queue
        info->pool->idles.put(info);
    }
    // never here
    return nullptr;
}

ThreadPool::ThreadPool(size_type num) : idles(){
    // initialize some threads
    for (size_type i = 0; i < num; ++i) {
        auto *info = createThread();
        idles.put(info);
    }
}

ThreadPool::info * ThreadPool::createThread() {
    struct info *info = new struct info(this);
    ++count;
    return info;
}

void ThreadPool::schedule(Callable *callee) {
    struct info *info;
    if (idles.empty()) {
        info = createThread();
    } else {
        info = idles.pop();
    }
    info->callee = callee;
    info->mutex.unlock();
}

ThreadPool::~ThreadPool() {
}

ThreadPool::info::info(ThreadPool *_this) : pool(_this) {
    mutex.lock();
    pthread_create(&pid, nullptr, pool_routine, this);
}

bool ThreadPool::info::equals(ThreadPool::info *rhs) {
    return this == rhs;
}
