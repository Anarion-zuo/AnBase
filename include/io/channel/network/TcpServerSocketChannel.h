//
// Created by anarion on 2020/2/22.
//

#ifndef MYCPPBASE_TCPSERVERSOCKETCHANNEL_H
#define MYCPPBASE_TCPSERVERSOCKETCHANNEL_H

#include "ServerSocketChannel.h"
#include "TcpSocketChannel.h"

namespace anarion {
    class TcpServerSocketChannel : virtual public ServerSocketChannel, public TcpSocketChannel {
    public:
        TcpServerSocketChannel(in_port_t port_num);

        void bind(in_port_t port_num) override;
        void listen(int backlog) override;
        int accept() override;
    };
}

#endif //MYCPPBASE_TCPSERVERSOCKETCHANNEL_H
