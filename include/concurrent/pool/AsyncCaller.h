//
// Created by anarion on 4/22/20.
//

#ifndef MYCPPBASE_ASYNCCALLER_H
#define MYCPPBASE_ASYNCCALLER_H

#include "ThreadPool.h"

namespace anarion {
    class AsyncCaller : protected ThreadPool {

        struct AsyncInfo {
            Callable *callable;
            int isDone = false;
            AsyncCaller *caller;  // inject this
            Mutex lock;
            CondVar cond;

            AsyncInfo(Callable *callable, AsyncCaller *caller);

            void wait() {
                lock.lock();
                cond.wait(isDone);
                // do nothing
                lock.unlock();
            }
        };

        class __routine : virtual public Callable {
            AsyncInfo *info;
        public:
            explicit __routine(AsyncInfo *info) : info(info) {}
            void run() override;
        };

        static void *routine(void *args) {
            // load info
//            AsyncInfo *info = static_cast<AsyncInfo *>(args);
//            // lock up
//            info->lock.lock();
//
//            // execute
//            info->callable->run();
//            delete info->callable;
//
//            // done state
//            info->isDone = true;
//            info->caller->removeInfo(info);
//
//            // signal
//            info->cond.broadcast();
//            info->lock.unlock();
//
//            delete info;
//            return nullptr;
        }

        HashSet<AsyncInfo*> infoSet;
        Mutex setLock;
        void registerInfo(AsyncInfo *info) {
            setLock.lock();
            infoSet.insert(info);
            setLock.unlock();
        }
        void removeInfo(AsyncInfo *info) {
            setLock.lock();
            infoSet.remove(info);
            setLock.unlock();
        }

    public:
        AsyncCaller() : ThreadPool(1) {}

        AsyncInfo *call(Callable &callable) {
            Callable *pcall = callable.forward();
            AsyncInfo *info = new AsyncInfo(pcall, this);
            registerInfo(info);
            schedule(new __routine(info));
            return info;
        }

        bool isDone(AsyncInfo *info) {
            setLock.lock();
            auto it = infoSet.find(info);
            if (it == infoSet.end_iterator()) {
                setLock.unlock();
                return false;
            }
            bool flag = info->isDone;
            setLock.unlock();
            return flag;
        }

        void wait(AsyncInfo *info) {
            if (info == nullptr) {
                return;
            }
            setLock.lock();
            auto it = infoSet.find(info);
            if (it == infoSet.end_iterator()) {
                setLock.unlock();
                return;
            }
            setLock.unlock();
            info->wait();
        }

    };
}

#endif //MYCPPBASE_ASYNCCALLER_H
