//
// Created by anarion on 2020/2/21.
//

#ifndef MYCPPBASE_OUTCHANNEL_H
#define MYCPPBASE_OUTCHANNEL_H

#include "Channel.h"

namespace anarion {
    class InChannel;
    class OutChannel : virtual public Channel {
    public:
        virtual size_type out(char *p, size_type nbytes) = 0;
        virtual Buffer out(size_type nbytes) = 0;
        virtual Buffer out() = 0;

        void transferTo(InChannel &dst);
    };

}

#endif //MYCPPBASE_OUTCHANNEL_H
