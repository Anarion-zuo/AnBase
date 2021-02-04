//
// Created by anarion on 2/4/21.
//

#ifndef ANBASE_WAITTHREAD_H
#define ANBASE_WAITTHREAD_H

#include "Thread.h"

namespace anarion {
    template<typename Callable>
    class WaitThread : public Thread<Callable> {
    protected:

        static void *start_wait_routine(void *p) {
            WaitThread<Callable> *thread = static_cast<WaitThread<Callable> *>(p);
            thread->waitGo();
            thread->routine();
        }

        Mutex mutex;
        bool go = false;
        Cond<Mutex> cond;
        void waitGo() {
            mutex.lock();
            cond.waitFor(go);
            go = false;
            mutex.unlock();
        }

    public:

        WaitThread() : mutex(), cond(mutex) {}
        WaitThread(WaitThread &&rhs) noexcept : Thread<Callable>(forward<Thread<Callable>>(rhs)), mutex(), cond(mutex) {}
        explicit WaitThread(Callable &&callable) : Thread<Callable>(forward<Callable>(callable)), mutex(), cond(mutex) {}
        explicit WaitThread(const Callable &callable) : Thread<Callable>(callable), mutex(), cond(mutex) {}

        void launchWait() {
            mutex.lock();
            go = true;
            mutex.unlock();
            cond.broadcast();
        }

        void startWait() {
            ThreadCore::startPThread(start_wait_routine);
        }

    };

    template <typename RoutineType>
    WaitThread<RoutineType> makeWaitThread(const RoutineType &routine) {
        return WaitThread<RoutineType>(routine);
    }

    template <typename RoutineType>
    WaitThread<RoutineType> makeWaitThread(RoutineType &&routine) {
        return WaitThread<RoutineType>(forward<RoutineType>(routine));
    }

}
#endif //ANBASE_WAITTHREAD_H
