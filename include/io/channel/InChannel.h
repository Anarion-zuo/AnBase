//
// Created by anarion on 2020/2/21.
//

#ifndef MYCPPBASE_INCHANNEL_H
#define MYCPPBASE_INCHANNEL_H

#include "Channel.h"


namespace anarion {
    class OutChannel;
    class InChannel : virtual public Channel {
    protected:
        bool i_valid;
    public:

        explicit InChannel(bool is_valid) : i_valid(is_valid) {}
        InChannel(InChannel &&rhs) noexcept : i_valid(rhs.i_valid) { rhs.i_valid = false; }

        virtual size_type in(char *p, size_type nbytes) = 0;
        virtual size_type in(Buffer &buffer) = 0;
        virtual size_type in(Buffer &buffer, size_type nbytes) = 0;

        virtual void closei() = 0;

        bool inOn() const { return i_valid; }
        void invalidi() { i_valid = false; }

        void transferFrom(OutChannel &src);
    };
}

#endif //MYCPPBASE_INCHANNEL_H
