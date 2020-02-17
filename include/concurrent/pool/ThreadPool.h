//
// Created by 左钰 on 2020/1/16.
//

#ifndef MYCPPLIB_THREADPOOL_H
#define MYCPPLIB_THREADPOOL_H


#include "../base/Thread.h"
#include "../base/Mutex.h"
#include "../../context/Callable.h"
#include "../../container/Queue/BlockQueue.hpp"

namespace anarion {
    class ThreadPool {
    protected:
        struct info {
            Callable *callee = nullptr;
            Mutex mutex;
            ThreadPool *pool;
            pthread_t pid;

            explicit info(ThreadPool *_this);
            bool equals(info *rhs);
        };

        BlockQueue<info*> idles;
        size_type count = 0;

        static void *pool_routine(void *p);

    public:
        explicit ThreadPool(size_type num);

        ~ThreadPool();

        info *createThread();
        void joinAll();
        void schedule(Callable *callee);
    };
}

#endif //MYCPPLIB_THREADPOOL_H
