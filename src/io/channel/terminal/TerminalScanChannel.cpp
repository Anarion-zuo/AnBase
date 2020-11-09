//
// Created by anarion on 4/11/20.
//

#include "io/channel/terminal/TerminalScanChannel.h"

void anarion::TerminalScanChannel::close() {

}

anarion::size_type anarion::TerminalScanChannel::out(char *p, anarion::size_type nbytes) {
    return readn(STDIN_FILENO, p, nbytes);
}

anarion::Buffer anarion::TerminalScanChannel::out(anarion::size_type nbytes) {
    Buffer buffer;
    buffer.append_fd(STDIN_FILENO, nbytes);
    return move(buffer);
}

anarion::Buffer anarion::TerminalScanChannel::out() {
    Buffer buffer;
    buffer.append_fd(STDIN_FILENO);
    return move(buffer);
}

anarion::TerminalScanChannel::TerminalScanChannel() {}
