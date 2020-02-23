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
    bool trylock();

    bool check();
};
}

#endif //MYCPPLIB_RWMUTEX_H
