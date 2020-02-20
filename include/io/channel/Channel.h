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
protected:
    bool is_valid;


public:

    Channel(bool is_valid) : is_valid(is_valid) {}
    Channel(Channel &&rhs) : is_valid(rhs.is_valid) { rhs.is_valid = false; }

    virtual size_type in(char *p, size_type nbytes) = 0;
    virtual size_type in(Buffer &buffer) = 0;
    virtual size_type in(Buffer &buffer, size_type nbytes) = 0;
    virtual size_type out(char *p, size_type nbytes) = 0;
    virtual Buffer out(size_type nbytes) = 0;
    
    bool valid() const { return is_valid; }

    virtual void close() = 0;
};
}

#endif  // CHANNEL_H