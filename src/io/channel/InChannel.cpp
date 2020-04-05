//
// Created by anarion on 2020/4/1.
//

#include <io/channel/InChannel.h>
#include <io/channel/OutChannel.h>

void anarion::InChannel::transferFrom(anarion::OutChannel &src) {
    Buffer buffer = src.out();
    in(buffer);
}
