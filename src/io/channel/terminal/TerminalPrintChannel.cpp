//
// Created by anarion on 4/11/20.
//

#include "io/channel/terminal/TerminalPrintChannel.h"

anarion::TerminalPrintChannel::TerminalPrintChannel() : InChannel(true) {}

anarion::size_type anarion::TerminalPrintChannel::in(char *p, anarion::size_type nbytes) {
    return writen(STDOUT_FILENO, p, nbytes);
}

anarion::size_type anarion::TerminalPrintChannel::in(anarion::Buffer &buffer) {
    return buffer.write_fd(STDOUT_FILENO, buffer.unread());
}

anarion::size_type anarion::TerminalPrintChannel::in(anarion::Buffer &buffer, anarion::size_type nbytes) {
    return buffer.write_fd(STDOUT_FILENO, nbytes);
}

void anarion::TerminalPrintChannel::closei() {

}

void anarion::TerminalPrintChannel::close() {

}
