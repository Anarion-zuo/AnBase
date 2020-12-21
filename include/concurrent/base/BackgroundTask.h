//
// Created by anarion on 5/3/20.
//

#ifndef ANBASE_BACKGROUNDTASK_H
#define ANBASE_BACKGROUNDTASK_H

#include "Thread.h"

namespace anarion {
    class BackgroundTask {
    protected:
        Time rollTime;
    public:
        explicit BackgroundTask(Time &&time) : rollTime(time) {}

        void run();
        virtual void task() = 0;
    };
}

#endif //ANBASE_BACKGROUNDTASK_H
