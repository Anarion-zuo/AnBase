//
// Created by anarion on 2020/2/23.
//

#ifndef MYCPPBASE_LISTENER_H
#define MYCPPBASE_LISTENER_H

class SocketChannel;

namespace anarion {
    class Listener {
    protected:
        int fd;
    public:

        virtual void run(int backlog) = 0;

        virtual void onRead(int cfd) = 0;
        virtual void onWrite(int cfd) = 0;
        virtual void onException(int cfd) = 0;

    };
}

#endif //MYCPPBASE_LISTENER_H
