//
// Created by anarion on 2020/2/20.
//

#include "CondVar.h"

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
    ::pthread_cond_signal(&cond);
}

void anarion::CondVar::broadcast() {
    ::pthread_cond_broadcast(&cond);
}
