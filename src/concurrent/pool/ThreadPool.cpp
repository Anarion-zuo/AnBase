//
// Created by 左钰 on 2020/1/16.
//

#include <concurrent/pool/ThreadPool.h>

using namespace anarion;

ThreadPool::ThreadPool(size_type num) : idleLock(), idleCond(idleLock) {
    addIdle(num);
}

ThreadPool::pool_ins *ThreadPool::createThread() {
    ::pthread_t pid;
    pool_ins *ins = new pool_ins(this, pid);
    int ret = ::pthread_create(&pid, nullptr, pool_routine, ins);
    if (ret) { throw ThreadStartException(); }
    ++count;
    setLock.lock();
    insSet.insert(ins);
    setLock.unlock();
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
            thread->cond.broadcast();
            return nullptr;
        }
//        thread->pool->running.insert(thread);

        thread->func->run();

        thread->pool->idleLock.lock();
        thread->pool->idles.push(thread);
        thread->pool->idleCond.signal();
        thread->pool->idleLock.unlock();

        thread->flag = false;
        delete thread->func;
        thread->cond.broadcast();
        thread->mutex.unlock();
    }
    return nullptr;
}

void ThreadPool::addIdle(size_type num) {
    for (size_type i = 0; i < num; ++i) {
        pool_ins *p = createThread();

        idleLock.lock();
        idles.push(p);
        idleCond.signal();
        idleLock.unlock();
    }
}

void ThreadPool::putBack(ThreadPool::pool_ins *ins) {
    idleLock.lock();
    idles.push(ins);
    idleCond.signal();
    idleLock.unlock();
}

/*
 * this has to be applied to an idle thread
 * pthread_join is not necessary
 * an idle thread would enter return right away
 */
void ThreadPool::joinIdleThread(ThreadPool::pool_ins *ins) {
    ins->flag = 2;
    ins->cond.broadcast();
}

/*
 * the procedure of running a thread is implemented as following:
 * 1) pop an instance from idle queue
 *      a) if the queue is empty, allocate one new
 *      b) if not, proceed
 * 2) load callable object to pool_ins
 * 3) signal
 */
//void ThreadPool::schedule(const Callable &callee) {
//    pool_ins *p = getIns();
//    delete p->func;
//    p->func = callee.clone();
//    p->flag = 1;
//    p->cond.signal();
//}

ThreadPool::~ThreadPool() {
    joinAll();
}

ThreadPool::pool_ins *ThreadPool::getIns() {
    pool_ins *p;
    idleLock.lock();
    if (idles.empty()) {
        p = createThread();
    } else {
        // dont have to wait with fore-hand check
        p = idles.pop();
    }
    idleLock.unlock();
    return p;
}

void ThreadPool::joinAll() {

    for (size_type i = 0; i < count; ++i) {
        idleLock.lock();
        while (idles.empty()) {
            idleCond.wait();
        }
        pool_ins *ins = idles.pop();
        idleLock.unlock();

        joinIdleThread(ins);

        setLock.lock();
        insSet.remove(ins);
        setLock.unlock();

        delete ins;
    }
    count = 0;
}

//void ThreadPool::schedule(func_type func_p, void *args) {
//    pool_ins *p = getIns();
//    p->func_p = func_p;
//    p->args_p = args;
//    p->flag = 1;
//    p->cond.signal();
//}

ThreadPool::pool_ins * ThreadPool::schedule(Callable &callable) {
    Callable *pcall = callable.forward();
    return schedule(pcall);
}

ThreadPool::pool_ins * ThreadPool::schedule(Callable *callable) {
    pool_ins *p = getIns();
    p->func = callable;
    p->flag = 1;
    p->cond.signal();
    return p;
}

bool ThreadPool::isRunning(ThreadPool::pool_ins *ins) {
    if (ins == nullptr) { return false; }
    setLock.lock();
    auto it = insSet.find(ins);
    if (it == insSet.end_iterator()) {
        setLock.unlock();
        return false;
    }
    setLock.unlock();
    return ins->flag;
}

void ThreadPool::wait(ThreadPool::pool_ins *ins) {
    if (!isRunning(ins)) {
        return;
    }
    ins->wait();
}

void ThreadPool::pool_ins::wait() {
    mutex.lock();
    while (flag == 1) {
        cond.wait();
    }
    mutex.unlock();
}
