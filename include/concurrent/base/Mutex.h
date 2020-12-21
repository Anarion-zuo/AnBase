//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_MUTEX_H
#define MYCPPLIB_MUTEX_H


#include <pthread.h>
#include <SystemException.h>
#include "../../feature/UnCopyable.h"

namespace anarion {
class Mutex : virtual public UnCopyable {
//    friend class CondVar<>;
protected:
    pthread_mutex_t mu;

public:

    Mutex();
    Mutex(Mutex &&rhs) noexcept ;
    ~Mutex();
    Mutex &operator=(Mutex &&rhs) noexcept ;

    pthread_mutex_t &getHandle();

    void lock();
    void unlock();
    bool trylock();

    bool check() ;
};

    class MutexException : public SystemException {

    };

    class MutexCreateException : public MutexException {

    };

    class MutexLockException : public MutexException {

    };

    class MutexUnlockException : public MutexException {

    };
}

#endif //MYCPPLIB_MUTEX_H
