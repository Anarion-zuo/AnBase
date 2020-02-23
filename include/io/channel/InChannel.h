//
// Created by anarion on 2020/2/21.
//

#ifndef MYCPPBASE_INCHANNEL_H
#define MYCPPBASE_INCHANNEL_H

#include "Channel.h"

namespace anarion {
    class InChannel : virtual public Channel {
    protected:
        bool i_valid;
    public:

        explicit InChannel(bool is_valid) : Channel(), i_valid(is_valid) {}
        InChannel(InChannel &&rhs) noexcept : Channel(forward<InChannel>(rhs)) { rhs.i_valid = false; }

        virtual size_type in(char *p, size_type nbytes) = 0;
        virtual size_type in(Buffer &buffer) = 0;
        virtual size_type in(Buffer &buffer, size_type nbytes) = 0;

        virtual void closei() = 0;

        bool inOn() const { return i_valid; }
    };
}

#endif //MYCPPBASE_INCHANNEL_H
