//
// Created by anarion on 4/11/20.
//

#include <concurrent/base/Thread.h>
#include "logger/Logger.h"


void anarion::Logger::processOne(anarion::LoggerInfo *info) {
    for (auto it = outputs.begin_iterator(); it != outputs.end_iterator(); ++it) {
        info->toChannel(**it);
    }
    delete info;
}

void anarion::Logger::roll() {

}

void anarion::Logger::setRefreshTime(anarion::size_type msec) {
    refreshTime.setMSec(msec);
}

void anarion::Logger::run() {
    while (true) {
        infoLock.lock();
        while (!infoQueue.empty()) {
            LoggerInfo *info = infoQueue.pop();
            processOne(info);
        }
        infoLock.unlock();
//        Thread::sleep(refreshTime);
    }
}
