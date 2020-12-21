//
// Created by anarion on 12/8/20.
//

#ifndef ANBASE_DATABASELOGENTRY_H
#define ANBASE_DATABASELOGENTRY_H

#include <serialize/logger/LogEntry.h>

namespace anarion {
namespace db {
class DataBaseLogEntry : public StringMessageLogEntry {
protected:
    SString getMessage() const override;
public:
    DataBaseLogEntry(SString &&message, LogLevel *logLevel = new InfoLogLevel);
};
}
}
#endif //ANBASE_DATABASELOGENTRY_H
