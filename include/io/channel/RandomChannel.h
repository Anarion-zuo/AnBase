#ifndef RANDOMCHANNEL_H
#define RANDOMCHANNEL_H

#include "Channel.h"

/**
 * @Name: RandomChannel
 * @Parent: Channel
 * @Utility: encapsulated file descriptor for system io with random access interface
 *          a) file
 *          b) mapped memory
 * @Interface:
 *      1) set current operating position
 *          a) reset to head
 *          b) reset to end
 *          c) move back and forth
 *          d) total block size
 *      3) 
 * 
 */

namespace anarion {
class RandomChannel : virtual public Channel {
protected:

    RandomChannel(bool is_valid) : Channel(is_valid) {}
    RandomChannel(RandomChannel &&rhs) = default;

public:

    // position
    virtual void rewind();
    virtual void set_append();
    virtual void move_forth(size_type nbytes);
    virtual void move_back(size_type nbytes);
    virtual size_type size() const = 0;

};
} // namespace anarion


#endif  // RANDOMCHANNEL_H