//
// Created by anarion on 2020/2/20.
//

#ifndef MYCPPBASE_SOCKETCHANNEL_H
#define MYCPPBASE_SOCKETCHANNEL_H

#include <netinet/in.h>
#include "io/channel/OutChannel.h"
#include "io/channel/InChannel.h"
#include <io/buffer/Buffer.h>

namespace anarion {
    class SocketChannel : virtual public OutChannel, virtual public InChannel {
    protected:
        int sockfd;
    public:
        explicit SocketChannel(int sockfd) : sockfd(sockfd) {}
        SocketChannel(SocketChannel &&rhs) noexcept : Channel(forward<SocketChannel>(rhs)), InChannel(forward<SocketChannel>(rhs)), OutChannel(forward<SocketChannel>(rhs)), sockfd(rhs.sockfd) {}

        void setNonBlock() const ;
        virtual void connect(const char *host_ip, in_port_t host_port) = 0;

        int getFd() const { return sockfd; }

        static void throwSocket();

        size_type in(const char *p, size_type nbytes) override;
        size_type in(Buffer &buffer) override;
        size_type in(Buffer &buffer, size_type nbytes) override;

        bool checkClose();

        size_type out(char *p, size_type nbytes) override;
        Buffer out(size_type nbytes) override;
        Buffer out() override ;

        size_type outUntil(char *buffer, size_type length, char c);

        //        Buffer outBuffer(size_type nbytes);
//        Buffer outBuffer();
    };
}

namespace anarion {
    class SocketException : public std::exception {
    protected:
        int error_num;
    public:
        explicit SocketException(int error_num) : error_num(error_num) {}
        const char *what() const noexcept override;
    };
    struct SocketConnectionRefusedException : public SocketException { explicit SocketConnectionRefusedException(int error_num) : SocketException(error_num) {} };
    struct SocketNetUnreachableException : public SocketException { explicit SocketNetUnreachableException(int error_num) : SocketException(error_num) {} };
    struct SocketHostUnreachableException : public SocketException { explicit SocketHostUnreachableException(int error_num) : SocketException(error_num) {} };
    struct SocketTimeOutException : public SocketException { explicit SocketTimeOutException(int error_num) : SocketException(error_num) {} };
    struct SocketConnectionResetException : public SocketException { explicit SocketConnectionResetException(int error_num) : SocketException(error_num) {} };

}

#endif //MYCPPBASE_SOCKETCHANNEL_H
