//
// Created by anarion on 2020/2/20.
//

#include "concurrent/base/CondVar.h"

anarion::CondVar::CondVar(anarion::Mutex &mutex) : mutex(mutex) {
    int ret;
    ret = ::pthread_cond_init(&cond, nullptr);
    if (ret) {
        throw CondVarCreateException();
    }
}

anarion::CondVar::CondVar(anarion::CondVar &&rhs) noexcept : mutex(rhs.mutex), cond(rhs.cond) {
    rhs.cond = ::pthread_cond_t{0};
}

anarion::CondVar &anarion::CondVar::operator=(anarion::CondVar &&rhs) noexcept {
    mutex = move(rhs.mutex);
    cond = rhs.cond;
    rhs.cond = ::pthread_cond_t{0};
    return *this;
}

void anarion::CondVar::signal() {
    int ret;
    ret = ::pthread_cond_signal(&cond);
    if (ret) {
        throw CondVarInvalid();
    }
}

void anarion::CondVar::broadcast() {
    int ret = ::pthread_cond_broadcast(&cond);
    if (ret) {
        throw CondVarInvalid();
    }
}

void anarion::CondVar::throwByReturn(int retVal) {
    switch (retVal) {
        case EINVAL:
            throw CondVarInvalid();
        case EPERM:
            throw CondVarInvalid();
        default:
            return;
    }
}
