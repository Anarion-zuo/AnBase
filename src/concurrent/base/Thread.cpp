//
// Created by 左钰 on 2020/1/15.
//

#include <concurrent/base/Thread.h>

using namespace anarion;
/*
void Thread::start() {
    int ret;
    ret = pthread_create(&pid, nullptr, start_routine, this);
    if (ret != 0) {
        throw ThreadStartException();
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
    if (ret) { throw ThreadJoinException(); }
}

Thread::Thread(Thread &&rhs) noexcept : pid(rhs.pid) {
    rhs.pid = pthread_t{0};
}

Thread &Thread::operator=(Thread &&rhs) noexcept {
    pid = rhs.pid;
    rhs.pid = pthread_t{0};
    return *this;
}
*/
void ThreadCore::sleep(const Time &sleepTime) {
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

void ThreadCore::startPThread(void *(*fp)(void *)) {
    int ret;
    ret = pthread_create(&pid, nullptr, fp, this);
    if (ret != 0) {
        throw ThreadStartException();
    }
}

void ThreadCore::joinPThread() const {
    int ret;
    ret = pthread_join(pid, nullptr);
    if (ret) { throw ThreadJoinException(); }
}

void ThreadCore::cancel() const {
    int ret = pthread_cancel(pid);
    if (ret < 0) {
        throw ThreadCancelFailed();
    }
}
