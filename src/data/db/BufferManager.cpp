//
// Created by anarion on 11/25/20.
//

#include "data/db/BufferManager.h"

anarion::db::BufferManager::BufferManager(anarion::db::bufferoff_t bufferSize, anarion::db::bufferno_t bufferInitCount)
    : bufferSize(bufferSize), bufferList(bufferInitCount), freeList(0) {
    allocateNewBuffer(bufferInitCount);
}

void anarion::db::BufferManager::allocateNewBuffer(bufferno_t count) {
    for (size_type index = 0; index < count; ++index) {
        char *p = static_cast<char *>(operator new(bufferSize));
        bufferList.push_back(BufferInfo {.head = p, .pin  =false});
    }
    freeList.append(count);
}

void anarion::db::BufferManager::deallocateBuffer(anarion::db::bufferno_t bufferno) {
    operator delete(bufferList.get(bufferno).head, bufferSize);
    bufferList.remove(bufferno);
}

char *anarion::db::BufferManager::getBufferAddress(anarion::db::bufferno_t bufferno) {
    if (bufferno >= getBufferCount()) {
        return nullptr;
    }
    return bufferList.get(bufferno).head;
}

const char *anarion::db::BufferManager::getBufferAddress(anarion::db::bufferno_t bufferno) const {
    if (bufferno >= getBufferCount()) {
        return nullptr;
    }
    return bufferList.get(bufferno).head;
}

void anarion::db::BufferManager::setPageno(anarion::db::bufferno_t bufferno, anarion::db::pageno_t pageno) {
    getBuffer(bufferno).pageno = pageno;
}

anarion::db::BufferManager::BufferInfo &anarion::db::BufferManager::getBuffer(anarion::db::bufferno_t bufferno) {
    return bufferList.get(bufferno);
}

anarion::db::pageno_t anarion::db::BufferManager::getBufferPageno(anarion::db::bufferno_t bufferno) const {
    return getBuffer(bufferno).pageno;
}

const anarion::db::BufferManager::BufferInfo &
anarion::db::BufferManager::getBuffer(anarion::db::bufferno_t bufferno) const {
    return bufferList.get(bufferno);
}
