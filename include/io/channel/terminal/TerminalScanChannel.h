//
// Created by anarion on 4/11/20.
//

#ifndef MYCPPBASE_TERMINALSCANCHANNEL_H
#define MYCPPBASE_TERMINALSCANCHANNEL_H

#include <io/channel/OutChannel.h>

namespace anarion {
    class TerminalScanChannel : public virtual OutChannel {
    public:
        TerminalScanChannel();

        size_type out(char *p, size_type nbytes) override;
        Buffer out(size_type nbytes) override;
        Buffer out() override;

        void close() override;

    };
}

#endif //MYCPPBASE_TERMINALSCANCHANNEL_H
