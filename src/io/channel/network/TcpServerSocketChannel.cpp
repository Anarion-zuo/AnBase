//
// Created by anarion on 2020/2/22.
//

#include "io/channel/network/TcpServerSocketChannel.h"

void anarion::TcpServerSocketChannel::bind(in_port_t port_num) {
    if (port_num == local_addr.sin_port) { return; }
    local_addr.sin_port = htons(port_num);
    int ret = ::bind(sockfd, reinterpret_cast<const sockaddr *>(&local_addr), sizeof(sockaddr_in));
    if (ret < 0) {

    }
}

void anarion::TcpServerSocketChannel::listen(int backlog) {
    ::listen(sockfd, backlog);
}

int anarion::TcpServerSocketChannel::accept() {
    int cfd = ::accept(sockfd, nullptr, nullptr);
    if (cfd < 0) {
        throwSocket();
    }
    return cfd;
}

anarion::TcpServerSocketChannel::TcpServerSocketChannel(in_port_t port_num) : InChannel(true), OutChannel(true), SocketChannel(0), TcpSocketChannel(), ServerSocketChannel(port_num) {
    ::memset(&local_addr, 0, sizeof(sockaddr_in));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(port_num);
    int ret = ::bind(sockfd, reinterpret_cast<const sockaddr *>(&local_addr), sizeof(sockaddr_in));
    if (ret < 0) { throwSocket(); }
}
