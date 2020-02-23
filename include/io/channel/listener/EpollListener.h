//
// Created by anarion on 2020/2/23.
//

#ifndef MYCPPBASE_EPOLLLISTENER_H
#define MYCPPBASE_EPOLLLISTENER_H

#include <io/channel/network/ServerSocketChannel.h>
#include <sys/epoll.h>
#include "Listener.h"

namespace anarion {
    class EpollListener : public Listener {
    protected:
        ServerSocketChannel *server;
        int epfd;
        epoll_event *events;

    public:

        explicit EpollListener(ServerSocketChannel *server);
        ~EpollListener();

        void run(int backlog) override;
    };
}

#endif //MYCPPBASE_EPOLLLISTENER_H
