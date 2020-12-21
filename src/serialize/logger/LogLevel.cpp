//
// Created by anarion on 12/5/20.
//

#include "serialize/logger/LogLevel.h"

anarion::SString anarion::FailureLogLevel::print() const {
    return SString("[failure]");
}

anarion::SString anarion::WarningLogLevel::print() const {
    return SString("[warning]");
}

anarion::SString anarion::DebugLogLevel::print() const {
    return SString("[debug]");
}

anarion::SString anarion::InfoLogLevel::print() const {
    return SString("[info]");
}
