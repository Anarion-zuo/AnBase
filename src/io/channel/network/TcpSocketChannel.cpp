//
// Created by anarion on 2020/2/22.
//

#include <exceptions/InvalidOperation.h>
#include <arpa/inet.h>
#include "io/channel/network/TcpSocketChannel.h"
#include "exceptions/io/socket/ConnectException.h"

void anarion::TcpSocketChannel::connect(const char *host_ip, in_port_t host_port) {
    if (!(i_valid && o_valid)) { throw InvalidOperation(); }
    sockaddr_in addr;
    ::memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_addr.s_addr = ::inet_addr(host_ip);
    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(host_port);
    int ret = ::connect(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
    if (ret < 0) { throwSocket(); }
}

void anarion::TcpSocketChannel::close() {
    closei();
    closeo();
}

void anarion::TcpSocketChannel::closei() {
    if (!i_valid) { return; }
    ::shutdown(sockfd, SHUT_WR);
    i_valid = false;
}

void anarion::TcpSocketChannel::closeo() {
    if (!o_valid) { return; }
    ::shutdown(sockfd, SHUT_RD);
    o_valid = false;
}

anarion::TcpSocketChannel::TcpSocketChannel() : InChannel(true), OutChannel(true), SocketChannel(0) {
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { throw SocketCreateException(); }
    // debug
    int ret = 1;
    ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(int));
}

anarion::TcpSocketChannel::TcpSocketChannel(int cfd) : InChannel(true), OutChannel(true), SocketChannel(cfd) {}
