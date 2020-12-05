//
// Created by anarion on 12/5/20.
//

#ifndef ANBASE_LOGLEVEL_H
#define ANBASE_LOGLEVEL_H

#include <container/SString.h>

namespace anarion {
class LogLevel {
public:
    LogLevel() = default;
    virtual ~LogLevel() = default;

    virtual SString print() const = 0;
};

class FailureLogLevel : public LogLevel {
public:
    SString print() const override;
};

class WarningLogLevel : public LogLevel {
public:
    SString print() const override;
};

class DebugLogLevel : public LogLevel {
public:
    SString print() const override;
};

class InfoLogLevel : public LogLevel {
public:
    SString print() const override;
};
}

#endif //ANBASE_LOGLEVEL_H
