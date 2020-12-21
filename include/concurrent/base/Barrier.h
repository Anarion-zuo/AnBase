//
// Created by anarion on 2020/2/21.
//

#ifndef MYCPPBASE_BARRIER_H
#define MYCPPBASE_BARRIER_H


#include "CondVar.h"

/*
 * Barrier class serve the purpose of synchronizing multiple procedures
 * call wait function to wait until all procedures have checked in
 * 1) thread calls checkin() to register itself
 * 2) thread immediately call wait() to wait for others to register
 * 3) irrelevant threads may call wait() to wait for all procedures to complete
 */

namespace anarion {
    class Barrier {
    protected:
        Mutex waitLock, checkLock;
        CondVar cond;

        size_type target_count, cur_count = 0;

        constexpr bool all_checked() const { return target_count == cur_count; }

    public:
        explicit Barrier(size_type target) : target_count(target), waitLock(), cond(waitLock) {}

        void checkin();
        void wait();

        /*
         * method reset must be used with utter caution
         * must not be called when the barrier is still in use
         */
        void reset(size_type target);
    };
}

#endif //MYCPPBASE_BARRIER_H
