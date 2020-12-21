//
// Created by anarion on 5/1/20.
//

#include <io/base/sys_utility.h>
#include <cstdlib>
#include "time/Time.h"

void anarion::Time::setCurrent() {
    clock_gettime(clockid, &this->kernelTime);
}

anarion::Time anarion::Time::now() {
    Time time;
    time.setCurrent();
    return time;
}

bool anarion::Time::operator<(const anarion::Time &rhs) const {
    if (kernelTime.tv_sec < rhs.kernelTime.tv_sec) {
        return true;
    }
    if (kernelTime.tv_sec > rhs.kernelTime.tv_sec) {
        return false;
    }
    return kernelTime.tv_nsec < rhs.kernelTime.tv_nsec;
}

bool anarion::Time::operator>(const anarion::Time &rhs) const {
    return rhs < *this;
}

bool anarion::Time::operator<=(const anarion::Time &rhs) const {
    return !(rhs < *this);
}

bool anarion::Time::operator>=(const anarion::Time &rhs) const {
    return !(*this < rhs);
}

timespec anarion::Time::difference(const anarion::Time &left, const anarion::Time &right) {
    timespec ret;
    if (left > right) {
        ret.tv_sec = left.kernelTime.tv_sec - right.kernelTime.tv_sec;
        if (left.kernelTime.tv_nsec > right.kernelTime.tv_nsec) {
            ret.tv_nsec = left.kernelTime.tv_nsec - right.kernelTime.tv_nsec;
        } else {
            --ret.tv_sec;
            ret.tv_nsec = (long)1e9 - (right.kernelTime.tv_nsec - left.kernelTime.tv_nsec);
        }
        return ret;
    }
    ret.tv_sec = right.kernelTime.tv_sec - left.kernelTime.tv_sec;
    if (right.kernelTime.tv_nsec > left.kernelTime.tv_nsec) {
        ret.tv_nsec = right.kernelTime.tv_nsec - left.kernelTime.tv_nsec;
    } else {
        --ret.tv_sec;
        --ret.tv_sec;
        ret.tv_nsec = (long)1e9 - (left.kernelTime.tv_nsec - right.kernelTime.tv_nsec);
    }
    return ret;
}



anarion::Time::Time(anarion::size_type sec, anarion::size_type nsec)
#ifdef __APPLE__
    : kernelTime({static_cast<__darwin_time_t>((sec)), static_cast<long>((nsec))})
#elif __linux__
    : kernelTime({static_cast<__time_t>(sec), static_cast<__syscall_slong_t>(nsec)})
#endif
    {
    if (nsec > 999999999) {
        throw NanoSecondRangeError();
    }
}

anarion::Time &anarion::Time::operator=(const anarion::Time &rhs) {
    if (this == &rhs) { return *this; }
    Copier<Time>().copy(this, &rhs, 1);
    return *this;
}

anarion::Time &anarion::Time::operator=(anarion::Time &&rhs) noexcept {
    return operator=(rhs);
}

anarion::Time::Time(const anarion::Time &rhs)
: kernelTime(rhs.kernelTime), clockid(rhs.clockid) {

}

double anarion::Time::getMsecField() const {
    return kernelTime.tv_nsec * 1.0e-6;
}


