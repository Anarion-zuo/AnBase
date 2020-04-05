//
// Created by anarion on 2020/4/1.
//

#include <io/channel/OutChannel.h>
#include <io/channel/InChannel.h>


void anarion::OutChannel::transferTo(anarion::InChannel &dst) {
    anarion::Buffer buffer = out();
    dst.in(buffer);
}
