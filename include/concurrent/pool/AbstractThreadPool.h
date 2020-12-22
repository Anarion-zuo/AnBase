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
    public:
        using entry_t = uint32_t;
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
                isFree = 3, // is waiting to be assigned a task
//                isBusy = 2,    // is executing some task
                isBusy = 1,    // is going to run the task
                isDead = 0,   // is exiting thread mainRoutine
            };
            enum State state = isDead;
            uint16_t refCount = 0;
            // pool info
            size_type poolIndex = -1;
            AbstractThreadPool *pool = nullptr;

            explicit ThreadEntry(enum State initState) : thread(), mutex(), cond(mutex), state(initState) {}

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
            void monitorWaitComplete();

            // set the state of this handle and pool
            void enterFree();
            void enterDead();
            void enterBusy();
            void leaveFree();
            void leaveDead();
            void leaveBusy();

            // entering and leaving a loop
            void acquireTaskEntrance();
            void releaseTaskEntrance();

            static void *entryRoutine(void *entryPointer);
        };

        void startMainRoutine(size_type index, ThreadEntry *entry);
        virtual void markBusy(size_type index, ThreadEntry *entry) = 0;
        virtual void markFree(size_type index, ThreadEntry *entry) = 0;
        virtual void markDead(size_type index, ThreadEntry *entry) = 0;
        virtual void unmarkBusy(size_type index, ThreadEntry *entry) = 0;
        virtual void unmarkFree(size_type index, ThreadEntry *entry) = 0;
        virtual void unmarkDead(size_type index, ThreadEntry *entry) = 0;

        Vector<ThreadEntry> entries;
        ThreadEntry &getEntry(size_type index);
        const ThreadEntry &getEntry(size_type index) const ;

    public:

        explicit AbstractThreadPool(size_type poolSize) : entries(poolSize, ThreadEntry::isDead) {}

        void start(size_type index);
        void launch(size_type index);
        void setTask(size_type index, PoolExecutable *executable);
        bool tryKill(size_type index);
        void forceKill(size_type index);
        void join(size_type index);

        bool isBusy(size_type index);
        bool isFree(size_type index);
        bool isDead(size_type index);

        struct Exception : public std::exception {};
        struct ThreadStateUnknown : public Exception {};
        struct ThreadAlreadyStarted : public Exception {};
        struct NullExecutable : public Exception {};
        struct EntryUsedByMultipleMonitor : public Exception {};
    };
}

#endif //ANBASE_ABSTRACTTHREADPOOL_H
