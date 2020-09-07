//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_THREAD_H
#define MYCPPLIB_THREAD_H


#include <pthread.h>
#include <time/Time.h>
#include <SystemException.h>
#include "Routine.h"

/*
 * Thread class template
 *
 * Real thread operations are done in ThreadCore, with Thread defining convenient calling interface.
 * Thread encapuslates a RoutineType, with viable generic interface run.
 * Routine's are for late function calls, storing parameters and procedure for calls at random time.
 * Thread prepares a function call in a RoutineType, and calls it when starting a thread by system call.
 *
 * Note:
 *  - passing reference to BindRoutine type may not work accross threads, best pass by pointer.
 */

namespace anarion {
    class ThreadCore {
    protected:
        pthread_t pid;
        ThreadCore() = default;

        void startPThread(void *(*fp)(void *));
        void joinPThread() const;

    public:
        static void sleep(const Time &sleepTime);

        void cancel() const;
    };

    template <typename RoutineType>
    class Thread : public ThreadCore {
    protected:

        static void *start_routine(void *p) {
            RoutineType &proutine = static_cast<Thread<RoutineType> *>(p)->routine;
            proutine.run();
            return nullptr;
        }

        RoutineType routine;

    public:

        Thread() = default;
        Thread(const Thread &) = default;
        Thread(Thread &&) noexcept = default;
        ~Thread() = default;
        Thread(const RoutineType &routine) : routine(routine) {}
        Thread(RoutineType &&routine) : routine(forward<RoutineType>(routine)) {}

        void start() {
            startPThread(start_routine);
        }
        void join() {
            joinPThread();
        }

        constexpr RoutineType &getRoutine() { return routine; }
        constexpr const RoutineType &getRoutine() const { return routine; }

    };

    template <typename RoutineType>
    Thread<RoutineType> makeThread(const RoutineType &routine) {
        return Thread<RoutineType>(routine);
    }

    template <typename RoutineType>
    Thread<RoutineType> makeThread(RoutineType &&routine) {
        return Thread<RoutineType>(forward<RoutineType>(routine));
    }

    class ThreadException : public SystemException {};
    class ThreadStartException : public ThreadException {};
    struct ThreadJoinException : public SystemException {};
    struct ThreadCancelFailed : public ThreadException {};

struct SleepError : public SystemException {
    const char *what() const noexcept override {
        return "Error when the thread is entering sleep.";
    }
};

    class AsyncCall {
    protected:

    public:
        void call();
    };
}

#endif //MYCPPLIB_THREAD_H
