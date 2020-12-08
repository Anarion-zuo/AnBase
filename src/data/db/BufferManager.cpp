//
// Created by anarion on 11/25/20.
//

#include "data/db/storage/BufferManager.h"
#include "data/db/storage/PageManager.h"

anarion::db::BufferManager::BufferManager(anarion::db::bufferoff_t bufferSize, anarion::db::bufferno_t bufferInitCount)
    : bufferSize(bufferSize),
    bufferList(bufferInitCount, {.head = nullptr, .refCount = 0, .pageno = PageManager::pagenoNull}),
    freeList(bufferInitCount) {
    allocateNewBuffer(bufferInitCount);
}

void anarion::db::BufferManager::allocateNewBuffer(bufferno_t count) {
    for (size_type index = 0; index < count; ++index) {
        char *p = static_cast<char *>(operator new(bufferSize));
        bufferList.get(index).head = p;
    }
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

anarion::db::bufferno_t anarion::db::BufferManager::pickEvict() {
    if (evictableList.size() == 0) {
        return null;
    }
    bufferno_t bufferno = evictableList.popLeastRecent();
    return bufferno;
}

void anarion::db::BufferManager::map(anarion::db::PageManager *pageManager, anarion::db::pageno_t pageno,
                                     anarion::db::bufferno_t bufferno) {
    BufferInfo &buffer = getBuffer(bufferno);
    buffer.pageManager = pageManager;
    buffer.pageno = pageno;
    buffer.refCount = 0;
}

void anarion::db::BufferManager::unmap(anarion::db::bufferno_t bufferno) {
    BufferInfo &buffer = getBuffer(bufferno);
    buffer.pageManager = nullptr;
    buffer.pageno = PageManager::pagenoNull;
    buffer.refCount = 0;
}

void anarion::db::BufferManager::markUsing(anarion::db::bufferno_t bufferno) {
    BufferInfo &buffer = getBuffer(bufferno);
    if (buffer.refCount == 0) {
        // extract from evict list
        evictableList.remove(bufferno);
    }
    buffer.refCount++;
}

void anarion::db::BufferManager::markNotUsing(anarion::db::bufferno_t bufferno) {
    BufferInfo &buffer = getBuffer(bufferno);
    if (buffer.refCount == 0) {
        return;
    }
    --buffer.refCount;
    if (buffer.refCount == 0) {
        evictableList.add(bufferno);
    }
}

anarion::db::bufferno_t anarion::db::BufferManager::allocateOne() {
    bufferno_t bufferno;
    bool success = freeList.fetch(bufferno);
    if (!success) {
        bufferno = pickEvict();
        evict(bufferno);
    }
    return bufferno;
}

void anarion::db::BufferManager::evict(anarion::db::bufferno_t bufferno) {
    BufferInfo &buffer = getBuffer(bufferno);
    if (buffer.refCount != 0) {
        throw EvictingPinnedPage();
    }
    buffer.pageManager->evictPage(buffer.pageno);
    unmap(bufferno);
}

anarion::db::bufferno_t anarion::db::BufferManager::mapPage(anarion::db::PageManager *pageManager, anarion::db::pageno_t pageno) {
    bufferno_t bufferno = allocateOne();
    map(pageManager, pageno, bufferno);
    beginUsing(bufferno);
    return bufferno;
}

void anarion::db::BufferManager::beginUsing(anarion::db::bufferno_t bufferno) {
    markUsing(bufferno);
}

void anarion::db::BufferManager::endUsing(anarion::db::bufferno_t bufferno) {
    markNotUsing(bufferno);
}
