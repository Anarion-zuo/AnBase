//
// Created by anarion on 12/30/20.
//

#ifndef ANBASE_COND_H
#define ANBASE_COND_H

#include "Mutex.h"
#include <pthread.h>
#include <time/Time.h>

namespace anarion {
    template <typename LockType>
    class Cond;

    template <>
    class Cond<Mutex> {
    private:
        pthread_cond_t handle;
        Mutex &mutex;

        void throwByReturn(int ret) {
            if (ret == 0) {
                return;
            }
            switch (ret) {
                case EINVAL:
                    throw CondHandleInvalid();
                case EPERM:
                    throw CondHandleInvalid();
                default:
                    return;
            }
        }

        void callPthreadCondWait() {
            throwByReturn(::pthread_cond_wait(&handle, &mutex.getHandle()));
        }

        void callPthreadCondTimeWait(const Time &time) {
            throwByReturn(::pthread_cond_timedwait(&handle, &mutex.getHandle(), &time.getSpecHandle()));
        }

    public:
        explicit Cond(Mutex &mutex) : mutex(mutex) {
            int ret;
            ret = ::pthread_cond_init(&handle, nullptr);
            if (ret) {
                throw CondInitException();
            }
        }

        ~Cond() { ::pthread_cond_destroy(&handle); }

        Cond(Cond &&rhs) noexcept : handle(rhs.handle), mutex(rhs.mutex) {}

        const pthread_cond_t &getHandle() const { return handle; }

        void waitRaw() {
            callPthreadCondWait();
        }

        template <typename Callback, typename ...Args>
        void waitRaw(Callback callback, Args ...args) {
            callPthreadCondWait();
            callback(args...);
        }

        void waitFor(bool &event) {
            while (!event) {
                callPthreadCondWait();
            }
        }

        template <typename EventType>
        void waitEvent(EventType event) {
            while (!event()) {
                callPthreadCondWait();
            }
        }

        template <typename EventType, typename Callback, typename ...Args>
        void waitEvent(EventType event, Callback callback, Args ...args) {
            while (!event()) {
                callPthreadCondWait();
            }
            callback(args...);
        }

        void waitTime(const Time &time) {
            callPthreadCondTimeWait(time);
        }

        template <typename EventType, typename Callback, typename ...Args>
        void waitTime(const Time &time, Callback callback, Args ...args) {
            callPthreadCondTimeWait(time);
            callback(args...);
        }

        void signal() {
            int ret = ::pthread_cond_signal(&handle);
            throwByReturn(ret);
        }

        void broadcast() {
            int ret = ::pthread_cond_broadcast(&handle);
            throwByReturn(ret);
        }

        struct Exception : public std::exception {};
        struct CondInitException : public Exception {};
        struct CondHandleInvalid : public Exception {};

    };
}

#endif //ANBASE_COND_H
