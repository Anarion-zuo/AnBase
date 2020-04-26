//
// Created by anarion on 4/22/20.
//

#include "concurrent/pool/AsyncCaller.h"

anarion::AsyncCaller::AsyncInfo::AsyncInfo(Callable *callable, AsyncCaller *caller)
        : callable(callable), caller(caller), lock(), cond(lock) {}

void anarion::AsyncCaller::__routine::run() {
    // lock up
    info->lock.lock();

    // execute
    info->callable->run();

    // done state
    info->isDone = true;
    info->caller->removeInfo(info);
    delete info->callable;

    // signal
    info->cond.broadcast();
    info->lock.unlock();

    delete info;
}
