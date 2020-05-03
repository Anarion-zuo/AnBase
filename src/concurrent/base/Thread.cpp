//
// Created by 左钰 on 2020/1/15.
//

#include <concurrent/base/Thread.h>
#include "exceptions/concurrent/ThreadCreateException.h"

using namespace anarion;

void Thread::start() {
    int ret;
    ret = pthread_create(&pid, nullptr, start_routine, this);
    if (ret != 0) {
        throw ThreadCreateException();
    }
}

void *Thread::start_routine(void *p) {
    Thread *_this = reinterpret_cast<Thread*>(p);
    _this->run();
    return nullptr;
}

Thread::Thread() {

}

void Thread::join() {
    int ret;
    ret = pthread_join(pid, nullptr);
}

Thread::Thread(Thread &&rhs) noexcept : pid(rhs.pid) {
    rhs.pid = pthread_t{0};
}

Thread &Thread::operator=(Thread &&rhs) noexcept {
    pid = rhs.pid;
    rhs.pid = pthread_t{0};
    return *this;
}

void Thread::sleep(const Time &sleepTime) {
    timespec remainedTime, beginTime = sleepTime.getSpecHandle();
    int ret;
    while (true) {
        ret = nanosleep(&beginTime, &remainedTime);
        if (ret < -1) {
            switch (errno) {
                case EINTR:
                    beginTime = remainedTime;
                    break;
                default:
                    throw SleepError();
            }
        } else {
            break;
        }
    }
}
