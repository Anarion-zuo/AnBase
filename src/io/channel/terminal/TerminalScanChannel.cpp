//
// Created by anarion on 4/11/20.
//

#include "io/channel/terminal/TerminalScanChannel.h"

void anarion::TerminalScanChannel::close() {

}

anarion::size_type anarion::TerminalScanChannel::out(char *p, anarion::size_type nbytes) {
    return readn(STDIN_FILENO, p, nbytes);
}

anarion::TerminalScanChannel::TerminalScanChannel() {}

void anarion::TerminalScanChannel::open() {

}

void anarion::TerminalScanChannel::closeOut() {

}
