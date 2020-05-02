//
// Created by anarion on 5/1/20.
//

#include <io/base/sys_utility.h>
#include <cstdlib>
#include "time/Time.h"

void anarion::Time::setCurrent() {
    clock_gettime(clockid, &this->kernelTime);
    setCalender();
}

anarion::Time anarion::Time::now() {
    Time time;
    time.setCurrent();
    return time;
}

void anarion::Time::setCalender() {
    if (isNull()) { return; }
    sec_type s = kernelTime.tv_sec;
    sec_type ns = kernelTime.tv_nsec;
    if (ns > 1e9 - 1e6) {
        ++s;
    }
    time_t timet = s;
    tm *tp = nullptr;
    if (timezone == GMT) {
        tp = localtime(&timet);
    } else if (timeZone == LocalDependent) {
        tp = gmtime(&timet);
    } else {

    }
    memcpy(&calenderTime, tp, sizeof(tm));
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
        ret.tv_nsec = left.kernelTime.tv_nsec - right.kernelTime.tv_nsec;
        return ret;
    }
    ret.tv_sec = right.kernelTime.tv_sec - left.kernelTime.tv_sec;
    ret.tv_nsec = right.kernelTime.tv_nsec - left.kernelTime.tv_nsec;
    return ret;
}

anarion::SString anarion::Time::print(const char *format) {
    char *ret = static_cast<char *>(operator new(256));
    size_type len = strftime(ret, 256, format, &calenderTime);
    return SString::move(ret, len);
}

anarion::SString anarion::Time::print() {
    // Fir, 01 May 2020 08:55:11 GMT
    return print("%a, %d %b %Y %T %Z");
}


