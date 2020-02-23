//
// Created by 左钰 on 2020/1/16.
//

#include "RwMutex.h"

using namespace anarion;

RwMutex::RwMutex() {
    int ret;
    ret = ::pthread_rwlock_init(&rwm, nullptr);
}

void RwMutex::rlock() {
    ::pthread_rwlock_rdlock(&rwm);
}

void RwMutex::unlock() {
    ::pthread_rwlock_unlock(&rwm);
}

bool RwMutex::trylock() {
    
}

void RwMutex::wlock() {
    pthread_rwlock_wrlock(&rwm);
}

RwMutex::~RwMutex() {
    pthread_rwlock_destroy(&rwm);
}

RwMutex::RwMutex(RwMutex &&rhs) noexcept : rwm(rhs.rwm) {
    rhs.rwm = pthread_rwlock_t{0};
}

RwMutex &RwMutex::operator=(RwMutex &&rhs) noexcept {
    rwm = rhs.rwm;
    rhs.rwm = pthread_rwlock_t{0};
    return *this;
}

bool RwMutex::check() {
    return false;
}
