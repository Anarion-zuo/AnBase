#ifndef CHANNEL_H
#define CHANNEL_H

#include "io/buffer/Buffer.h"
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
protected:
    void throwInvalidOperation(const char *functionName);
public:
    virtual void open() = 0;
    virtual void close() = 0;
    virtual ~Channel() = default;
};
}

#endif  // CHANNEL_H