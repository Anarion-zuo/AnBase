//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_CONDVAR_H
#define MYCPPLIB_CONDVAR_H


#include "Mutex.h"
#include "../../context/functors/Functor.h"
#include <pthread.h>
#include <container/Bind/binded.hpp>

/*
 * The condition variable type is arranged in a most convenient way that I can ever think of.
 * By the well-celebrated knowledge of concurrence programming, the use of condition variable must follow the rules:
 * 1) acquire lock before waiting
 * 2) wait in a loop
 * 3) release lock after signal
 * No. 1&3 must be implemented outside the design of this class, manually and with cautions.
 */

namespace anarion {
class CondVar : virtual public UnCopyable {
protected:
    pthread_cond_t cond;
    Mutex &mutex;

    void throwByReturn(int retVal);
public:
    explicit CondVar(Mutex &mutex);
    ~CondVar() { ::pthread_cond_destroy(&cond); }
    CondVar(CondVar &&rhs) noexcept ;
    CondVar &operator=(CondVar &&rhs) noexcept ;

    /*
     * An event is represented by a callable object.
     * The callable object must follow the semantic of taking no input and return a boolean type.
     * A return value of true suggests the event has taken place and waiting requirement is met.
     * It is encouraged to use the bind adapting mechanism to pass as the event.
     * Once the event is met and a lock is acquired, the lock must be released to prevent deadlock.
     */
    template <typename binded_t>
    void wait(binded_t event) {
        int ret;
        while (!event()) {
            ret = ::pthread_cond_wait(&cond, &mutex.getHandle());
            throwByReturn(ret);
        }
    }

    void wait(const int &flag) {
        while (!flag) {
            int ret = ::pthread_cond_wait(&cond, &mutex.getHandle());
            throwByReturn(ret);
        }
    }

    /*
     * bare wait without any binding
     */
    void wait() {
        int ret = ::pthread_cond_wait(&cond, &mutex.getHandle());
        throwByReturn(ret);
    }

    void waitTime(const timespec &timespec) {
        int ret = ::pthread_cond_timedwait(&cond, &mutex.getHandle(), &timespec);
        throwByReturn(ret);
    }

    void signal();
    void broadcast();
};

    class CondVarException : public SystemException {};
    struct CondVarCreateException : public CondVarException {};
    struct CondVarInvalid : public CondVarException {};

}

#endif //MYCPPLIB_CONDVAR_H
