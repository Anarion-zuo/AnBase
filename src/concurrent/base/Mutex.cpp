//
// Created by 左钰 on 2020/1/15.
//

#include <concurrent/base/Mutex.h>

using namespace anarion;

Mutex::Mutex() {
    int ret;
    ret = pthread_mutex_init(&mu, nullptr);
    if (ret) {
        throw MutexCreateException();
    }
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&mu);
}

void Mutex::lock() {
    int ret;
    ret = pthread_mutex_lock(&mu);
    if (ret) {
        throw MutexLockException();
    }
}

void Mutex::unlock() {
    int ret;
    ret = pthread_mutex_unlock(&mu);
    if (ret) {
        throw MutexUnlockException();
    }
}

bool Mutex::trylock() {
    int ret;
    ret = pthread_mutex_trylock(&mu);
    if (ret == EBUSY) {
        return false;
    }
    if (ret) {
        throw MutexLockException();
    }
    return true;
}

Mutex::Mutex(Mutex &&rhs) noexcept : mu(rhs.mu) {
    rhs.mu = pthread_mutex_t{0};   // rhs becomes invalid
}

Mutex &Mutex::operator=(Mutex &&rhs) noexcept {
    mu = rhs.mu;
    rhs.mu = pthread_mutex_t{0};
    return *this;
}

const pthread_mutex_t &Mutex::getHandle() const {
    return mu;
}

bool Mutex::check() {
    bool flag = trylock();
    if (flag) { unlock(); }
    return flag;
}
