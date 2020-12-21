//
// Created by 左钰 on 2020/1/16.
//

#include <concurrent/base/RwMutex.h>

using namespace anarion;

static void throwWithReturn(int retVal) {
    switch (retVal) {

    }
}

RwMutex::RwMutex() {
    int ret;
    ret = ::pthread_rwlock_init(&rwm, nullptr);
    if (ret) { throw RwMutexCreateException(); }
}

void RwMutex::rlock() {
    int ret = ::pthread_rwlock_rdlock(&rwm);
    if (ret) { throw RwMutexReadLockFailed(); }
}

void RwMutex::unlock() {
    int ret = ::pthread_rwlock_unlock(&rwm);
    if (ret) { throw RwMutexUnlockFailed(); }
}

void RwMutex::wlock() {
    int ret = ::pthread_rwlock_wrlock(&rwm);
    if (ret) { throw RwMutexWriteLockFailed(); }
}

RwMutex::~RwMutex() {
    ::pthread_rwlock_destroy(&rwm);
}

RwMutex::RwMutex(RwMutex &&rhs) noexcept : rwm(rhs.rwm) {
    rhs.rwm = pthread_rwlock_t{0};
}

RwMutex &RwMutex::operator=(RwMutex &&rhs) noexcept {
    rwm = rhs.rwm;
    rhs.rwm = pthread_rwlock_t{0};
    return *this;
}

