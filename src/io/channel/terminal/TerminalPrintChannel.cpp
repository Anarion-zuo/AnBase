//
// Created by anarion on 4/11/20.
//

#include "io/channel/terminal/TerminalPrintChannel.h"

anarion::TerminalPrintChannel::TerminalPrintChannel() {}

anarion::size_type anarion::TerminalPrintChannel::in(const char *p, anarion::size_type nbytes) {
    return writen(STDOUT_FILENO, (void *) p, nbytes);
}

void anarion::TerminalPrintChannel::close() {

}

void anarion::TerminalPrintChannel::closeIn() {

}

void anarion::TerminalPrintChannel::open() {

}
