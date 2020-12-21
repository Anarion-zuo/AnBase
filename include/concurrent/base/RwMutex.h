//
// Created by 左钰 on 2020/1/16.
//

#ifndef MYCPPLIB_RWMUTEX_H
#define MYCPPLIB_RWMUTEX_H


#include "Mutex.h"

namespace anarion {
class RwMutex : virtual public UnCopyable {
protected:
    pthread_rwlock_t rwm;

public:
    RwMutex();
    ~RwMutex();
    RwMutex(RwMutex &&rhs) noexcept ;
    RwMutex &operator=(RwMutex &&rhs) noexcept ;

    void rlock();
    void wlock();
    void unlock();
};

struct RwMutexException : public SystemException {};
struct RwMutexInvalid : public RwMutexException {};
struct RwMutexCreateException : public RwMutexException {};
struct RwMutexReadLockFailed : public RwMutexException {};
struct RwMutexUnlockFailed : public RwMutexException {};
struct RwMutexWriteLockFailed : public RwMutexException {};
}

#endif //MYCPPLIB_RWMUTEX_H
