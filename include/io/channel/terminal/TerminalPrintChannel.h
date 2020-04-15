//
// Created by anarion on 4/11/20.
//

#ifndef MYCPPBASE_TERMINALPRINTCHANNEL_H
#define MYCPPBASE_TERMINALPRINTCHANNEL_H

#include <io/channel/InChannel.h>

namespace anarion {
    class TerminalPrintChannel : public virtual InChannel {
    public:
        TerminalPrintChannel();

        size_type in(char *p, size_type nbytes) override;

        size_type in(Buffer &buffer) override;
        size_type in(Buffer &buffer, size_type nbytes) override;

        void closei() override;
        void close() override;
    };
}

#endif //MYCPPBASE_TERMINALPRINTCHANNEL_H
