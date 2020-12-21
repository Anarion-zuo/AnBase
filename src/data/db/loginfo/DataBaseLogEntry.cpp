//
// Created by anarion on 12/8/20.
//

#include "data/db/loginfo/DataBaseLogEntry.h"

anarion::SString anarion::db::DataBaseLogEntry::getMessage() const {
    return SString::concat({SString("andb "), message});
}

anarion::db::DataBaseLogEntry::DataBaseLogEntry(anarion::SString &&message, anarion::LogLevel *logLevel)
        : StringMessageLogEntry(forward<SString>(message), logLevel) {}
