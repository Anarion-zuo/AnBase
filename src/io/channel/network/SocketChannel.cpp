//
// Created by anarion on 2020/2/20.
//

#include <fcntl.h>
#include <exceptions/InvalidOperation.h>
#include <io/channel/network/SocketChannel.h>
#include <io/base/io-exceptions.h>

void anarion::SocketChannel::setNonBlock() const {
    if (!o_valid) { InvalidOperation(); }
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    if (flags < 0) { throw FcntlException(); }
    flags |= O_NONBLOCK;
    flags = ::fcntl(sockfd, F_SETFL, flags);
    if (flags < 0) { throw FcntlException(); }
}

void anarion::SocketChannel::throwSocket() {
    int __errno = errno;
    switch (__errno) {
        case ECONNREFUSED: throw SocketConnectionRefusedException(__errno);
        case ENETUNREACH:  // treat as EHOSTUNREACH
        case EHOSTUNREACH: throw SocketHostUnreachableException(__errno);
        case ETIMEDOUT: throw SocketTimeOutException(__errno);
        case ECONNRESET: throw SocketConnectionResetException(__errno);
        default: throw SocketException(__errno);
    }
}

anarion::size_type anarion::SocketChannel::in(char *p, anarion::size_type nbytes) {
    if (!i_valid) { throw InvalidOperation(); }
    return sendn(sockfd, p, nbytes, 0);
}

anarion::size_type anarion::SocketChannel::in(anarion::Buffer &buffer) {
    if (!i_valid) { throw InvalidOperation(); }
    return buffer.send_fd(sockfd, buffer.unread(), 0);
}

anarion::size_type anarion::SocketChannel::in(anarion::Buffer &buffer, anarion::size_type nbytes) {
    if (!i_valid) { throw InvalidOperation(); }
    return buffer.send_fd(sockfd, nbytes, 0);
}

anarion::size_type anarion::SocketChannel::out(char *p, anarion::size_type nbytes) {
    if (!o_valid) { throw InvalidOperation(); }
    return recvn(sockfd, p, nbytes, 0);
}

anarion::Buffer anarion::SocketChannel::out() {
    if (!o_valid) { throw InvalidOperation(); }
    Buffer buffer;
    buffer.recv_fd(sockfd, 0);
    return move(buffer);
}

anarion::Buffer anarion::SocketChannel::out(anarion::size_type nbytes) {
    if (!o_valid) { throw InvalidOperation(); }
    Buffer buffer(nbytes);
    buffer.recv_fd(sockfd, 0);
    return move(buffer);
}

anarion::size_type anarion::SocketChannel::outUntil(char *buffer, anarion::size_type length, char c) {
    for (size_type i = 0; i < length; ++i) {
        char cur;
        int len = recv(sockfd, &cur, 1, MSG_PEEK);
        if (len == 0) { return i; }
        if (len < 0) { return i; }
        recv(sockfd, &cur, 1, 0);
        if (cur == c) { return i; }
        buffer[i] = cur;
    }
    return length;
}

bool anarion::SocketChannel::checkClose() {
    char oneC;
    int len = recv(sockfd, &oneC, 1, MSG_PEEK);
    if (len == 1) { return false; }
    return true;
}

//anarion::Buffer anarion::SocketChannel::outBuffer(anarion::size_type nbytes) {
//    if (!o_valid) { InvalidOperation(); }
//    Buffer buffer(nbytes);
//    buffer.recv_fd(sockfd, 0);
//    return move(buffer);
//}
//
//anarion::Buffer anarion::SocketChannel::outBuffer() {
//    if (!o_valid) { InvalidOperation(); }
//    Buffer buffer;
//    buffer.recv_fd(sockfd, 0);
//    return move(buffer);
//}


const char *anarion::SocketException::what() const noexcept {
    return strerror(errno);
}
