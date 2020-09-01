//
// Created by anarion on 2020/2/21.
//

#ifndef MYCPPBASE_INCHANNEL_H
#define MYCPPBASE_INCHANNEL_H

#include "Channel.h"


namespace anarion {
    class OutChannel;
    class InChannel : virtual public Channel {
    public:
        virtual size_type in(const char *p, size_type nbytes) = 0;
        virtual size_type in(Buffer &buffer) = 0;
        virtual size_type in(Buffer &buffer, size_type nbytes) = 0;

        void transferFrom(OutChannel &src);
    };
}

#endif //MYCPPBASE_INCHANNEL_H
