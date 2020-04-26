//
// Created by anarion on 4/11/20.
//

#include "logger/Logger.h"

void anarion::Logger::printAInfo(const LoggerInfo &info) {
    if (outputChannel) {
        Buffer buffer = info.serialize();
        outputChannel->in(buffer);
    } else {
        throw LoggerNullOutputChannel();
    }
}

void anarion::Logger::printInfo(const SString &info) {

}

const char *anarion::LoggerNullOutputChannel::what() const noexcept {
    return "This logger instance has null channel pointer. Try setChannel() to specify a output channel for this logger.";
}
