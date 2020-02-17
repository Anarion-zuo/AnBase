//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_MUTEX_H
#define MYCPPLIB_MUTEX_H


#include <pthread.h>
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

    virtual void lock();
    virtual void unlock();
    virtual bool trylock();
};
}

#endif //MYCPPLIB_MUTEX_H
