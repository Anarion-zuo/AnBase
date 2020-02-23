//
// Created by 左钰 on 2020/1/16.
//

#include "ThreadPool.h"

using namespace anarion;

ThreadPool::ThreadPool(size_type num) {
    addIdle(num);
}

ThreadPool::pool_ins *ThreadPool::createThread() {
    ::pthread_t pid;
    pool_ins *ins = new pool_ins(this, pid);
    ::pthread_create(&pid, nullptr, pool_routine, ins);
    ++count;
    return ins;
}

/*
 * routine loaded to pthread_create
 * 1) wait for monitor signal to proceed
 * 2) run loaded callable object
 * 3) put thread back to idle queue
 * 4) restore state
 */
void *ThreadPool::pool_routine(void *p) {
    pool_ins *thread = reinterpret_cast<pool_ins*>(p);
    while (true) {
        thread->mutex.lock();
        thread->cond.wait(thread->flag);
        if (thread->flag == 2) {
            return nullptr;
        }
//        thread->pool->running.insert(thread);

        thread->func->run();

//        thread->pool->running.remove(thread);
        thread->pool->idles.push(thread);
        thread->flag = false;
        thread->mutex.unlock();
    }
    return nullptr;
}

void ThreadPool::addIdle(size_type num) {
    for (size_type i = 0; i < num; ++i) {
        pool_ins *p = createThread();
        idles.push(p);
    }
}

void ThreadPool::putBack(ThreadPool::pool_ins *ins) {
    idles.push(ins);
}

/*
 * this has to be applied to an idle thread
 * pthread_join is not necessary
 * an idle thread would enter return right away
 */
void ThreadPool::joinIdleThread(ThreadPool::pool_ins *ins) {
    ins->flag = 2;
    ins->cond.broadcast();
//    ::pthread_join(ins->pid, nullptr);
}

/*
 * the procedure of running a thread is implemented as following:
 * 1) pop an instance from idle queue
 *      a) if the queue is empty, allocate one new
 *      b) if not, proceed
 * 2) load callable object to pool_ins
 * 3) signal
 */
void ThreadPool::schedule(const Callable &callee) {
    pool_ins *p = getIns();
    delete p->func;
    p->func = callee.clone();
    p->flag = 1;
    p->cond.signal();
}

ThreadPool::~ThreadPool() {
    joinAll();
}

ThreadPool::pool_ins *ThreadPool::getIns() {
    pool_ins *p;
    if (idles.empty()) {
        p = createThread();
    } else {
        p = idles.pop();
    }
    return p;
}

void ThreadPool::joinAll() {

    for (size_type i = 0; i < count; ++i) {
        pool_ins *ins = idles.pop();
        joinIdleThread(ins);
        delete ins;
    }
    count = 0;
}
