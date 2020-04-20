//
// Created by 左钰 on 2020/1/16.
//

#ifndef MYCPPLIB_THREADPOOL_H
#define MYCPPLIB_THREADPOOL_H


#include "../base/Thread.h"
#include "../base/Mutex.h"
#include "../../context/Callable.h"
#include "concurrent/container/Stack/BlockStack.hpp"
#include "container/Set/HashSet.hpp"

namespace anarion {

    class ThreadPool {
        friend struct pool_ins;
    protected:

        struct pool_ins {
            Callable *func = nullptr;
            Mutex mutex;
            CondVar cond;
            ThreadPool *pool;
            pthread_t pid;

            /*
             * flag implies the state the thread should be
             * 0 -- block and wait
             * 1 -- normal run
             * 2 -- return and exit
             */
            int flag = false;

            explicit pool_ins(ThreadPool *_this, pthread_t pid) : pool(_this), pid(pid), cond(mutex) {}

            ~pool_ins() {
                delete func;
            }
        };

        BlockStack<pool_ins*> idles;
//        HashSet<pool_ins*> running;
        size_type count = 0;

        static void *pool_routine(void *p);
        static void joinIdleThread(pool_ins *ins);

        pool_ins *createThread();
        void addIdle(size_type num);
        void putBack(pool_ins *ins);
        pool_ins *getIns();

    public:
        explicit ThreadPool(size_type num);

        ~ThreadPool();

        void joinAll();
        void schedule(const Callable &callee);
    };


}

#endif //MYCPPLIB_THREADPOOL_H
