//
// Created by anarion on 2020/2/22.
//

#ifndef MYCPPBASE_TCPSOCKETCHANNEL_H
#define MYCPPBASE_TCPSOCKETCHANNEL_H

#include "ServerSocketChannel.h"

namespace anarion {
    class TcpSocketChannel : virtual public SocketChannel {
    protected:
        HostInfo info;
    public:
        TcpSocketChannel();
        explicit TcpSocketChannel(int cfd);
        TcpSocketChannel(TcpSocketChannel &&rhs) noexcept : Channel(forward<TcpSocketChannel>(rhs)), InChannel(forward<TcpSocketChannel>(rhs)), OutChannel(forward<TcpSocketChannel>(rhs)), SocketChannel(forward<TcpSocketChannel>(rhs)), info(rhs.info) {}

        /*
         * connect encapsulates connect system call
         * @Params:
         *      host_ip: takes a string of ipv4 address
         *      host_port: port
         * @Exceptions:
         *      Connection Refused: no process is listening at the specified address:port of the target host, while the host is reachable
         *      Time Out: the request of synchronization is not acked for a considerable long time
         */
        void connect(const char *host_ip, in_port_t host_port) override ;

        /*
         * close functions encapsulates shutdown system call
         * shutdown may close partly the writing or reading pipe associated with the socket
         */
        void close() override;
        void closei() override ;
        void closeo() override ;

        void setInfo(const HostInfo &info) { this->info = info; }
        HostInfo &getInfo() { return info; }
    };
}

#endif //MYCPPBASE_TCPSOCKETCHANNEL_H
