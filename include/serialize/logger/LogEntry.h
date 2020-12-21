//
// Created by anarion on 12/2/20.
//

#ifndef ANBASE_LOGENTRY_H
#define ANBASE_LOGENTRY_H

#include <serialize/Serializer.h>
#include <time/Date.h>
#include "LogLevel.h"

namespace anarion {

class LogEntry : public Serializable {
protected:

    Time time;
    Date date;
    LogLevel *logLevel;

    SString dateString() const ;
    SString space() const ;
    virtual SString getMessage() const = 0;

public:
    explicit LogEntry(LogLevel *logLevel = new InfoLogLevel)
    : time(Time::now()), date(time), logLevel(logLevel) {}
    virtual ~LogEntry();

    SString serialize() const override;
};

class StringMessageLogEntry : public LogEntry {
protected:
    SString message;

    SString getMessage() const override;

public:
    explicit StringMessageLogEntry(SString &&message, LogLevel *logLevel = new InfoLogLevel)
    : LogEntry(logLevel), message(forward<SString>(message)) {}
};
}

#endif //ANBASE_LOGENTRY_H
