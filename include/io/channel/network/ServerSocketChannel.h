//
// Created by anarion on 2020/2/22.
//

#ifndef MYCPPBASE_SERVERSOCKETCHANNEL_H
#define MYCPPBASE_SERVERSOCKETCHANNEL_H

#include "SocketChannel.h"
#include <container/SString.h>
#include <arpa/inet.h>

namespace anarion {

    struct HostInfo {
        char ip_str[INET_ADDRSTRLEN];
        uint16_t portNum;

        HostInfo() = default;
        explicit HostInfo(const sockaddr_in &client_addr) {
            inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
            portNum = ntohs(client_addr.sin_port);
        }
        HostInfo(const HostInfo &rhs) : portNum(rhs.portNum) {
            memcpy(ip_str, rhs.ip_str, INET_ADDRSTRLEN);
        }

        SString getIpString() {
            return SString(ip_str);
        }
    };

    class ServerSocketChannel : virtual public SocketChannel {
    protected:
        sockaddr_in local_addr;
    public:
        explicit ServerSocketChannel(in_port_t port_num);

        virtual void bind(in_port_t port_num) = 0;
        virtual void listen(int backlog) = 0;
        virtual int accept() = 0;
        virtual HostInfo acceptWithInfo() = 0;

        in_port_t getPortNum() const { return local_addr.sin_port; }
    };
}

#endif //MYCPPBASE_SERVERSOCKETCHANNEL_H
