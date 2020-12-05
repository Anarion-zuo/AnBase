//
// Created by anarion on 12/2/20.
//

#ifndef ANBASE_SERIALIZER_H
#define ANBASE_SERIALIZER_H

#include <io/channel/InChannel.h>
#include <concurrent/container/Queue/BlockQueue.hpp>

namespace anarion {
class Serializable {
public:
    virtual SString serialize() const = 0;
};
class Serializer {
protected:
    ListQueue<Serializable *> uncommittedQueue;
    Mutex commitLock, queueLock;

    static void commitOne(anarion::Serializable *obj, InChannel &inChannel);
public:
    void add(Serializable *obj);
    void commit(InChannel &inChannel);

    /**
     * @brief commit to serveral Channels at once.
     * @tparam Iterator Iterator of some container. Must be dereferenced to be type InChannel*.
     * @param begin
     * @param end
     */
    template <typename Iterator>
    void commit(Iterator begin, Iterator end) {
        queueLock.lock();
        commitLock.lock();
        while (!uncommittedQueue.empty()) {
            Serializable *serializable = uncommittedQueue.pop();
            for (Iterator it = begin; it != end; ++it) {
                commitOne(serializable, **it);
            }
        }
        commitLock.unlock();
        queueLock.unlock();
    }
};
}

#endif //ANBASE_SERIALIZER_H
