//
// Created by anarion on 2020/2/21.
//

#include "concurrent/base/Barrier.h"

/*
 *
 */

void anarion::Barrier::checkin() {
    checkLock.lock();
    ++cur_count;
    if (cur_count == target_count) {
        cond.broadcast();
    }
    checkLock.unlock();
}

void anarion::Barrier::wait() {
    waitLock.lock();
//    cond.wait(bind(&Barrier::all_checked, *this));
    waitLock.unlock();
}

void anarion::Barrier::reset(size_type target) {
    cur_count = 0;
    target_count = target;
}
