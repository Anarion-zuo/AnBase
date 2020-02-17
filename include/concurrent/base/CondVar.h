//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_CONDVAR_H
#define MYCPPLIB_CONDVAR_H


#include "Mutex.h"
#include "../../context/functors/Functor.h"
#include <pthread.h>
#include "../../exceptions/concurrent/CondVarCreateException.h"

namespace anarion {
template <typename cond_func> class CondVar : virtual public UnCopyable {
protected:
    pthread_cond_t cond;
    Mutex mutex;
    cond_func func;
public:
    explicit CondVar() {
        int ret;
        ret = pthread_cond_init(&cond, nullptr);
        if (ret) {
            throw CondVarCreateException();
        }
    }

    ~CondVar() {
        pthread_cond_destroy(&cond);
    }

    CondVar(CondVar &&rhs) noexcept : mutex(std::move(rhs.mutex)), cond(rhs.cond) {
        rhs.cond = pthread_cond_t{0};
    }
    CondVar &operator=(CondVar &&rhs) noexcept {
        mutex = std::move(rhs.mutex);
        cond = rhs.cond;
        rhs.cond = pthread_cond_t{0};
        return *this;
    }

    cond_func &getFuncObject() { return func; }

    void wait() {
        int ret;
        if (func()) {
            mutex.lock();
            return;
        }
        mutex.lock();
        while (true) {   // use double check when competition is fierce
            ret = pthread_cond_wait(&cond, &mutex.getHandle());
            if (ret) {

            }
            if (func()) {
                break;
            }
        }
    }

    void signal() {
        int ret;
        ret = pthread_cond_signal(&cond);
        if (ret) {

        }
    }

    void signalAll() {
        int ret;
        ret = pthread_cond_broadcast(&cond);
        if (ret) {

        }
    }

    void lock() {
        mutex.lock();
    }

    void unlock() {
        mutex.unlock();
    }
};
}

#endif //MYCPPLIB_CONDVAR_H
