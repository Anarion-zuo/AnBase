//
// Created by anarion on 5/3/20.
//

#ifndef ANBASE_BACKGROUNDTASK_H
#define ANBASE_BACKGROUNDTASK_H

#include "Thread.h"

namespace anarion {
    class BackgroundTask : public Thread {
    protected:
        Time rollTime;
    public:
        explicit BackgroundTask(Time &&time) : rollTime(time) {}

        void run() override;
        virtual void task() = 0;
    };
}

#endif //ANBASE_BACKGROUNDTASK_H
