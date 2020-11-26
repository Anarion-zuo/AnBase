//
// Created by anarion on 11/25/20.
//

#include "data/db/BufferManager.h"
#include "data/db/PageManager.h"

anarion::db::BufferManager::BufferManager(anarion::db::bufferoff_t bufferSize, anarion::db::bufferno_t bufferInitCount)
    : bufferSize(bufferSize),
    bufferList(bufferInitCount, {.head = nullptr, .pin = false, .pageno = PageManager::pagenoNull}),
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

anarion::db::bufferno_t
anarion::db::BufferManager::allocatePinned(anarion::db::pageno_t pageno, PageManager *pageManager) {
    bufferno_t bufferno = allocate(pageno, pageManager);
    if (bufferno != null) {
        pin(bufferno);
    }
    return bufferno;
}

void anarion::db::BufferManager::pin(anarion::db::bufferno_t bufferno) {
    if (!evictableList.remove(bufferno)) {
        // buffer not pinned
    } else {
        // buffer just pinned
        getBuffer(bufferno).pin = true;
    }
}

void anarion::db::BufferManager::unpin(anarion::db::bufferno_t bufferno) {
    evictableList.add(bufferno);
    getBuffer(bufferno).pin = false;
}

anarion::db::bufferno_t anarion::db::BufferManager::allocate(pageno_t pageno, PageManager *pageManager) {
    bufferno_t bufferno;
    if (freeList.fetch(bufferno)) {
        // there is a free buffer
    } else {
        // must evict a page
        bufferno = pickEvict();
        if (bufferno == null) {
            // no page to evict
            return null;
        }
        pageManager->evictPage(pageno);
    }
    getBuffer(bufferno).pageno = pageno;
    return bufferno;
}

anarion::db::bufferno_t
anarion::db::BufferManager::allocateUnpinned(anarion::db::pageno_t pageno, PageManager *pageManager) {
    bufferno_t bufferno = allocate(pageno, pageManager);
    if (bufferno != null) {
        unpin(bufferno);
    }
    return bufferno;
}

anarion::db::bufferno_t anarion::db::BufferManager::pickEvict() {
    if (evictableList.size() == 0) {
        return null;
    }
    bufferno_t bufferno = evictableList.popLeastRecent();
    return bufferno;
}
