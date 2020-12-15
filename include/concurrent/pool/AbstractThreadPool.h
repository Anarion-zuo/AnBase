//
// Created by anarion on 12/15/20.
//

#ifndef ANBASE_ABSTRACTTHREADPOOL_H
#define ANBASE_ABSTRACTTHREADPOOL_H

#include <concurrent/base/CondVar.h>
#include "PoolExecutable.h"
#include "../base/Thread.h"

namespace anarion {

    class AbstractThreadPool {
        /**
         * How the monitor controls the threads
         *
         * At init stage, monitor initializes an array of ThreadEntry, acting as handles. At the same time, it starts all the threads into busy state.
         *
         * At destroy stage, monitor waits for all threads to finish their current task, then kill them all.
         *
         * At normal running stage, monitor can do the following things.
         * - Mounting a task.
         *      - Set attribute executable.
         *      - Set thread state to pending.
         *      - Signal the condition variable.
         * - Starting off a thread mainRoutine.
         * - Ending a thread routing.
         */
    protected:
        struct ThreadEntry {
            // task info
            PoolExecutable *executable = nullptr;
            // concurrency control
            Mutex mutex;
            CondVar cond;
            ThreadCore thread;
            // thread state
            enum State {
                isPending = 3, // is waiting to be assigned a task
                isBusy = 2,    // is executing some task
                isReady = 1,    // is going to run the task
                isDead = 0,   // is exiting thread mainRoutine
            };
            enum State state = isDead;
            uint16_t refCount = 0;
            // pool info
            size_type poolIndex = -1;
            AbstractThreadPool *pool = nullptr;

            ThreadEntry() : thread(), mutex(), cond(mutex) {}

            void mainRoutine();
            void monitorStartRoutine();
            void monitorSetExecutable(PoolExecutable *e);
            /*
             * These 2 methods cannot behave as designed if called in the middle of an execution.
             *
             * An already launched task surely cannot be launched again.
             */
            bool monitorLaunchTask();
            bool monitorKillUnBusyRoutine();

            // set the state of this handle and pool
            void enterBusy();
            void enterPending();
            void enterDead();
            void enterReady();
            // entering and leaving a loop
            void acquireTaskEntrance();
            void releaseTaskEntrance();

            static void *entryRoutine(void *entryPointer);
        };

        void startMainRoutine(size_type index, ThreadEntry *entry);
        virtual void markBusy(size_type index, ThreadEntry *entry) = 0;
        virtual void markPending(size_type index, ThreadEntry *entry) = 0;
        virtual void markDead(size_type index, ThreadEntry *entry) = 0;
        virtual void markReady(size_type index, ThreadEntry *entry) = 0;

    public:

        struct Exception : public std::exception {};
        struct ThreadStateUnknown : public Exception {};
        struct ThreadAlreadyStarted : public Exception {};
        struct NullExecutable : public Exception {};
        struct EntryUsedByMultipleMonitor : public Exception {};
    };
}

#endif //ANBASE_ABSTRACTTHREADPOOL_H
