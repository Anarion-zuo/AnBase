//
// Created by anarion on 5/3/20.
//

#include "concurrent/base/BackgroundTask.h"


void anarion::BackgroundTask::run() {
    while (true) {
        task();
        sleep(rollTime);
    }
}
