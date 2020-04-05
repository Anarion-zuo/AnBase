#ifndef CHANNEL_H
#define CHANNEL_H

#include "../buffer/Buffer.h"
#include "unistd.h"

/**
 * @Name: Channel
 * @Parent: none
 * @Utility: general interface of system io
 *      1) random access
 *      2) rw stream access
 *      3) r stream access
 * @Interface:
 *      1) input/output
 *          a) buffer
 *          b) array
 *      3) close io channel
 *          a) check validity
 *          b) invalid exception
 * 
 */

namespace anarion {
class Channel {
public:
    virtual void close() = 0;

};
}

#endif  // CHANNEL_H