//
// Created by anarion on 12/2/20.
//

#include "serialize/logger/LogEntry.h"

anarion::SString anarion::LogEntry::serialize() const {
    return SString::concat({ logLevel->print(), dateString(), SString(" "), getMessage() });
}

anarion::SString anarion::LogEntry::space() const {
    return SString(" ", 1);
}

anarion::SString anarion::LogEntry::dateString() const {
    return SString::concat({ date.print("%Y %b %d %T:"), SString::parseDec(time.getMsecField(), 2) });
}

anarion::LogEntry::~LogEntry() {
    delete logLevel;
}

anarion::SString anarion::StringMessageLogEntry::getMessage() const {
    return message;
}

