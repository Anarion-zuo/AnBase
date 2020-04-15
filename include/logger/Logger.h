//
// Created by anarion on 4/11/20.
//

#ifndef MYCPPBASE_LOGGER_H
#define MYCPPBASE_LOGGER_H

#include <io/channel/InChannel.h>

namespace anarion {
    class LoggerInfo {
    public:
        virtual Buffer serialize() const = 0;
    };

    class Logger {
    protected:
        InChannel *outputChannel = nullptr;

    public:

        virtual void printAInfo(const LoggerInfo &info);
        constexpr void setChannel(InChannel *outputChannel) { this->outputChannel = outputChannel; }
    };

    struct LoggerNullOutputChannel : public std::exception {
        const char *what() const noexcept override;
    };


}

#endif //MYCPPBASE_LOGGER_H
