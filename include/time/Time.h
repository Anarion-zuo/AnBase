//
// Created by anarion on 5/1/20.
//

#ifndef ANBASE_TIME_H
#define ANBASE_TIME_H

#include <cstring>
#include <ctime>
#include <container/SString.h>

namespace anarion {
    class Time {
        using sec_type = unsigned long;
    protected:

        /*
         * support Linux high precision time mechanism
         * contains accurate measurement on nano-sec
         */
        timespec kernelTime;
        clockid_t clockid = CLOCK_REALTIME;
        /*
         * Select a timer from system definition
         *  CLOCK_REALTIME
         *  CLOCK_BOOTTIME
         *  CLOCK_PROCESS_CPUTIME_ID
         *  CLOCK_THREAD_CPUTIME_ID
         */

        void setNull() { memset(&kernelTime, 0, sizeof(timespec)); }

    public:

        constexpr const timespec &getSpecHandle() const { return kernelTime; }

        constexpr bool isNull() const { return kernelTime.tv_nsec == 0 && kernelTime.tv_sec == 0; }

        // constructors
        Time() { setNull(); }  // initialize a null object
        Time(size_type sec, size_type nsec);
        explicit Time(timespec timespec) : kernelTime(timespec) {}
        explicit Time(time_t msec) : kernelTime({0, msec * 1000000}) {}
        Time(const Time &rhs);
        Time(Time &&) noexcept = default;

        Time &operator=(const Time &rhs);
        Time &operator=(Time &&rhs) noexcept ;

        static Time now();

        // setters
        void setCurrent();
        constexpr void setClockType(clockid_t id) { clockid = id; }
        constexpr void setTimeSpec(const timespec &newTime) { kernelTime = newTime; }
        constexpr void setSecondField(size_type seconds) { kernelTime.tv_sec = seconds; }
        constexpr void setNanoSecondField(size_type nanosec) { kernelTime.tv_nsec = nanosec; }
        constexpr void setTime(size_type seconds, size_type nanosec) {
            setSecondField(seconds);
            setNanoSecondField(nanosec);
        }
        constexpr void setMSec(size_type msec) {
            size_type sec = msec / 1000;
            size_type nsec = msec % 1000 * 1000000;
            setTime(sec, nsec);
        }

        // time field
        constexpr sec_type getSecField() const { return kernelTime.tv_sec; }
        constexpr sec_type getNsecField() const { return kernelTime.tv_nsec; }
        constexpr double getDouble() const { return getSecField() + getNsecField() / 1e9; }
        double getMsecField() const ;

        /*
         * Operators
         * compare to check who is earlier
         * compute difference
         */
        bool operator<(const Time &rhs) const;
        bool operator>(const Time &rhs) const;
        bool operator<=(const Time &rhs) const;
        bool operator>=(const Time &rhs) const;

        static timespec difference(const Time &left, const Time &right);
    };

    template <>
    struct pod_traits<Time> {
        using is_pod = true_type;
    };

    template <>
    struct move_traits<Time> {
        using has_move_ctor = false_type;
    };

struct NanoSecondRangeError : public virtual std::exception {
    const char *what() const noexcept override {
        return "Overflowing nano-second field when setting time. Nano second should be set lower than 999999999.";
    }
};
}

#endif //ANBASE_TIME_H
