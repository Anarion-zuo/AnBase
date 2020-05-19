//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_THREAD_H
#define MYCPPLIB_THREAD_H


#include <pthread.h>
#include <time/Time.h>
#include <SystemException.h>
#include "../../context/Callable.h"
#include "../../feature/UnCopyable.h"

namespace anarion {
    class Thread : virtual public Callable, virtual public UnCopyable {
    protected:
        pthread_t pid;

        static void *start_routine(void *p);

    public:

        Thread();
        Thread(Thread &&rhs) noexcept;

        Thread &operator=(Thread &&rhs) noexcept;

        void start();
        void join();   // dont join yourself

        static void sleep(const Time &sleepTime);

    };

    class ThreadException : public SystemException {};
    class ThreadStartException : public ThreadException {};
    struct ThreadJoinException : public SystemException {};

struct SleepError : public SystemException {
    const char *what() const noexcept override {
        return "Error when the thread is entering sleep.";
    }
};
}

#endif //MYCPPLIB_THREAD_H
