//
// Created by anarion on 12/2/20.
//

#include <container/SString.h>
#include "serialize/Serializer.h"

void anarion::Serializer::add(anarion::Serializable *obj) {
    queueLock.lock();
    uncommittedQueue.push(obj);
    queueLock.unlock();
}

void anarion::Serializer::commitOne(anarion::Serializable *obj, InChannel &inChannel) {
    SString text = obj->serialize();
    inChannel.in(text.cstr(), text.length());
    inChannel.in("\n", 1);
}

void anarion::Serializer::commit(anarion::InChannel &inChannel) {
    queueLock.lock();
    commitLock.lock();
    while (!uncommittedQueue.empty()) {
        commitOne(uncommittedQueue.pop(), inChannel);
    }
    commitLock.unlock();
    queueLock.unlock();
}
