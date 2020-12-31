//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_THREAD_H
#define MYCPPLIB_THREAD_H


#include <pthread.h>
#include <time/Time.h>
#include <SystemException.h>
#include "Routine.h"


namespace anarion {
    /**
     * Thread class template
     *
     * Real thread operations are done in ThreadCore, with Thread defining convenient calling interface.
     * Thread encapuslates a Callable, with viable generic interface run.
     * Routine's are for late function calls, storing parameters and procedure for calls at random time.
     * Thread prepares a function call in a Callable, and calls it when starting a thread by system call.
     *
     * Note:
     *  - passing reference to BindRoutine type may not work accross threads, best pass by pointer.
     */

    class ThreadCore {
    protected:

        pthread_t pid;

    public:

        void startPThread(void *(*fp)(void *));
        void startPThread(void *(*fp)(void *), void *arg);
        void joinPThread() const;

        ThreadCore() = default;
        ThreadCore(ThreadCore &&rhs) noexcept ;
        ThreadCore(const ThreadCore &) = delete;

        ThreadCore &operator=(const ThreadCore &) = delete;
        ThreadCore &operator=(ThreadCore &&) noexcept = delete;

        static void sleep(const Time &sleepTime);

        void cancel() const;
    };

    template <typename Callable>
    class Thread : public ThreadCore {
    protected:

        static void *start_routine(void *p) {
            static_cast<Thread<Callable> *>(p)->routine();
            return nullptr;
        }

        Callable routine;

    public:

        Thread() = default;
        Thread(Thread &&) noexcept = default;
        ~Thread() = default;
        Thread(const Callable &routine) : routine(routine) {}
        Thread(Callable &&routine) : routine(forward<Callable>(routine)) {}

        void start() {
            startPThread(start_routine);
        }
        void join() {
            joinPThread();
        }

        constexpr Callable &getRoutine() { return routine; }
        constexpr const Callable &getRoutine() const { return routine; }

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
