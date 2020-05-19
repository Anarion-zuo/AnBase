//
// Created by anarion on 5/11/20.
//

#ifndef ANBASE_DYNAMICTHREADPOOL_H
#define ANBASE_DYNAMICTHREADPOOL_H

#include <concurrent/base/BackgroundTask.h>
#include "ThreadPool.h"

namespace anarion {
    class DynamicThreadPool : public ThreadPool {
    struct CleanThread : public BackgroundTask {
        DynamicThreadPool *pool;
    public:
        CleanThread(Time &&time, DynamicThreadPool *pool);
        void task() override;
    } cleanThread;

    public:
        DynamicThreadPool();
    };
}

#endif //ANBASE_DYNAMICTHREADPOOL_H
