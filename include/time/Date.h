//
// Created by anarion on 5/3/20.
//

#ifndef ANBASE_DATE_H
#define ANBASE_DATE_H

#include "Time.h"

namespace anarion {
    class Date {
        typedef unsigned long sec_type;
        /*
         * Calender time
         * translate time to real-life calender format from precise kernel time
         * any change in kernel time must reflect on calender time
         */
        struct tm calenderTime{};

        void setNull() {
            memset(&calenderTime, 0, sizeof(tm));
        }

        void setCalender(const timespec &kernelTime);  // initialize calenderTime according to kernelTime
        enum TimeZone { LocalDependent, GMT } timeZone = LocalDependent;

    public:
        Date() { setNull(); }
        explicit Date(const Time &timeObj);

        bool isNull();

        constexpr const tm &getCalenderHandle() const { return calenderTime; }
        constexpr void setTimeZone(enum TimeZone zone) { timeZone = zone; }
        void setTime(const Time &time) { setCalender(time.getSpecHandle()); }

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
    };
}

#endif //ANBASE_DATE_H
