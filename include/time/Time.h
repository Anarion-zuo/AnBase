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

        /*
         * Calender time
         * translate time to real-life calender format from precise kernel time
         * any change in kernel time must reflect on calender time
         */
        struct tm calenderTime{};

        void setNull() {
            memset(&kernelTime, 0, sizeof(timespec));
            memset(&calenderTime, 0, sizeof(tm));
        }

        void setCalender();  // initialize calenderTime according to kernelTime
        enum TimeZone { LocalDependent, GMT } timeZone = LocalDependent;

    public:

        constexpr const timespec &getSpecHandle() const { return kernelTime; }
        constexpr const tm &getCalenderHandle() const { return calenderTime; }
        constexpr bool isNull() const { return kernelTime.tv_nsec == 0 && kernelTime.tv_sec == 0; }

        // constructors
        Time() { setNull(); }  // initialize a null object
        explicit Time(timespec timespec) : kernelTime(timespec) { setCalender(); }
        explicit Time(time_t timet) : kernelTime({timet, 0}) { setCalender(); }
        Time(const Time &) = default;
        Time(Time &&) noexcept = default;

        static Time now();

        // setters
        void setCurrent();
        constexpr void setClockType(clockid_t id) { clockid = id; }
        constexpr void setTimeZone(enum TimeZone zone) { timeZone = zone; }

        // time field
        constexpr sec_type getSecField() const { return kernelTime.tv_sec; }
        constexpr sec_type getNsecField() const { return kernelTime.tv_nsec; }
        constexpr double getDouble() const { return getSecField() + getNsecField() / 1e9; }

        // calender
        constexpr int getDateSec() const { return calenderTime.tm_sec; }
        constexpr int getMinute() const { return calenderTime.tm_min; }
        constexpr int getHour() const { return calenderTime.tm_hour; }
        constexpr int getDayInMonth() const { return calenderTime.tm_mday; }
        constexpr int getMonth() const { return calenderTime.tm_mon; }
        constexpr int getYear() const { return calenderTime.tm_year; }
        constexpr int getDayInWeek() const { return calenderTime.tm_wday; }
        constexpr int getDayInYear() const { return calenderTime.tm_yday; }

        // format
        // man strftime for detailed format instructions
        SString print(const char *format);
        SString print();

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
}

#endif //ANBASE_TIME_H
