#ifndef RANDOMCHANNEL_H
#define RANDOMCHANNEL_H

#include "OutChannel.h"
#include "InChannel.h"

namespace anarion {
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
class RandomChannel : virtual public InChannel, virtual public OutChannel {
public:
    // position
    virtual void resetCursor() = 0;
    virtual void setCursorAppend() = 0;
    virtual void moveForthCursor(size_type nbytes) = 0;
    virtual void moveBackCursor(size_type nbytes) = 0;
    virtual void setCursor(size_type index) = 0;
    virtual size_type size() const = 0;

    void closeIn() override {
        throwInvalidOperation(__FUNCTION__);
    }

    void closeOut() override {
        throwInvalidOperation(__FUNCTION__);
    }
};
} // namespace anarion


#endif  // RANDOMCHANNEL_H