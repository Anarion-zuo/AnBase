//
// Created by anarion on 2020/2/22.
//

#ifndef MYCPPBASE_SERVERSOCKETCHANNEL_H
#define MYCPPBASE_SERVERSOCKETCHANNEL_H

#include "SocketChannel.h"

namespace anarion {
    class ServerSocketChannel : virtual public SocketChannel {
    protected:
        sockaddr_in local_addr;
    public:
        ServerSocketChannel(in_port_t port_num);

        virtual void bind(in_port_t port_num) = 0;
        virtual void listen(int backlog) = 0;
        virtual int accept() = 0;
    };
}

#endif //MYCPPBASE_SERVERSOCKETCHANNEL_H
