//
// Created by anarion on 2020/2/23.
//

#include "EpollListener.h"
#include <sys/epoll.h>
#include "exceptions/io/socket/EpollException.h"

static void addEvent(int epfd, int fd, int state) {
    epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    if (ret < 0) { throw EpollException(); }
}

static void delEvent(int epfd, int fd, int state) {
    epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
    if (ret < 0) { throw EpollException(); }
}

void anarion::EpollListener::run(int backlog) {
    epfd = ::epoll_create(backlog);
    addEvent(epfd, server->getFd(), EPOLLIN);
    int ret, fd, lfd = server->getFd();
    while (true) {
        ret = epoll_wait(epfd, events, backlog, 0);   // non block
        if (ret < 0) { throw EpollException(); }
        // traverse array
        for (int index = 0; index < ret; ++index) {
            fd = events[index].data.fd;
            if ((events[index].events & EPOLLIN) == 0) {
                if ((events[index].events & EPOLLERR)) {
                    onException(fd);
                }
                continue;
            }
            if (fd == lfd) {
                // accept
                sockaddr_in client_addr;
                socklen_t addrlen;
                int cfd = ::accept(lfd, reinterpret_cast<sockaddr *>(&client_addr), &addrlen);
                if (cfd == -1) {
                    throw EpollException();
                }
                addEvent(epfd, cfd, EPOLLIN);
                continue;
            }
            // read
//            Buffer *buffer = newObject<Buffer>();
            Buffer buffer;
            bool close_flag;
            // normal read
            close_flag = buffer.recv_fd(fd, 0);
            // exception

            // close tunnel
            if (!close_flag) {
                // may alter
                delEvent(epfd, fd, EPOLLIN);
            }
                // hand on and care not
            else {
//                dispatch(move(buffer), TcpSocketChannel(fd));
            }
            if ((events[index].events & EPOLLOUT)) {
                onWrite(fd);
            }
        }
    }
}

anarion::EpollListener::EpollListener(ServerSocketChannel *server) : server(server) {

}

anarion::EpollListener::~EpollListener() {
    delete server;
}
