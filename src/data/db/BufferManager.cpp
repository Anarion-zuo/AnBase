//
// Created by anarion on 11/25/20.
//

#include "data/db/storage/BufferManager.h"
#include "data/db/storage/PageManager.h"


void anarion::db::Buffer::allocateBytes(size_type count) {
    bytes = static_cast<char *>(operator new(count));
}

void anarion::db::Buffer::deallocateBytes(size_type count) {
    operator delete (bytes, count);
}

void anarion::db::Buffer::bindPage(anarion::db::PageManager *pageManager1, anarion::db::pageno_t pageno1) {
    pageManager = pageManager1;
    pageno = pageno1;
}
/*
bool anarion::db::Buffer::validBind() {
    try {
        return pageManager->getPage(pageno).bufferno == pageManager->getBufferManager().getBufferno(this);
    } catch (std::exception &) {
        return false;
    }
}*/

anarion::db::BufferManager::BufferManager(anarion::db::bufferoff_t pageSize, anarion::db::bufferno_t bufferCount)
: bufferSize(pageSize), bufferList(bufferCount), freeList(bufferCount), evictableListLock(), evictableCond(evictableListLock)/*, pinnedList(bufferCount)*/ {
    for (bufferno_t index = 0; index < bufferCount; ++index) {
        bufferList.emplaceBack()->allocateBytes(pageSize);
    }
}

anarion::db::BufferManager::~BufferManager() {
    for (auto it = bufferList.beginIterator(); it != bufferList.endIterator(); ++it) {
        it->deallocateBytes(bufferSize);
    }
}

anarion::db::bufferno_t anarion::db::BufferManager::popFree() {
    bufferno_t bufferno;
    freeListLock.lock();
    if (!freeList.fetch(bufferno)) {
        freeListLock.unlock();
        throw FreeListExhausted();
    }
    freeListLock.unlock();
    return bufferno;
}

anarion::db::bufferno_t anarion::db::BufferManager::popEvictable() {
    evictableListLock.lock();
    bufferno_t bufferno = evictableList.popLeastRecent();
    evictableListLock.unlock();
    return bufferno;
}

void anarion::db::BufferManager::pushFree(anarion::db::bufferno_t bufferno) {
    Buffer &buffer = getBuffer(bufferno);
    freeListLock.lock();
    buffer.stateLock.lock();

    freeList.putBack(bufferno);
    buffer.state = Buffer::Free;

    buffer.stateLock.unlock();
    freeListLock.unlock();
}

void anarion::db::BufferManager::pushEvictable(anarion::db::bufferno_t bufferno) {
    Buffer &buffer = getBuffer(bufferno);
    evictableListLock.lock();
    buffer.stateLock.lock();

    evictableList.add(bufferno);
    getBuffer(bufferno).state = Buffer::Evictable;

    buffer.stateLock.unlock();
    evictableListLock.unlock();
}

void anarion::db::BufferManager::loadPage(anarion::db::PageManager &pageManager, anarion::db::pageno_t pageno) {
    evictableListLock.lock();

    Page &page = pageManager.getPage(pageno);
    if (page.bufferno < bufferList.size()) {
        Buffer &buffer = getBuffer(page.bufferno);

        buffer.stateLock.lock();
        if (buffer.pageno == pageno && buffer.pageManager == &pageManager) {
            // the buffer is for this page
            if (buffer.state == Buffer::Evictable) {
                // formerly mapped to a evictable buffer
                // can be used directly
                evictableList.remove(page.bufferno);
                buffer.state = Buffer::Pinned;

                // return this special case
                buffer.stateLock.unlock();
                evictableListLock.unlock();
                return;
            }
            if (buffer.state == Buffer::Pinned) {
                // buffer already fetched
                // return this special case
                buffer.stateLock.unlock();
                evictableListLock.unlock();
                return;
            }
            // a buffer in the free list points to a page
            buffer.stateLock.unlock();
            evictableListLock.unlock();
            throw FreeHangingBuffer();
        }
        buffer.stateLock.unlock();
    }
    // this buffer is not for this page
    // must find another by freeList or evictList

    // fetch buffer
    freeListLock.lock();
    bufferno_t newno;
    bool success = freeList.fetch(newno);
    freeListLock.unlock();
    if (!success) {
        // must evict
        /*try {*/
            while (evictableList.size() == 0) {
                evictableCond.waitRaw();
            }
            newno = evictableList.popLeastRecent();
        /*} catch (LinkedList<bufferno_t>::Underflow &) {
            // evictable list exhausted
            // cannot allocate anymore
            throw WholePoolExhausted();
        }*/
        evict(newno);
    }
    // newno must be out of both free and evictable list by now
    // which means newno is pinned
    Buffer &newbuf = getBuffer(newno);
    newbuf.stateLock.lock();
    newbuf.state = Buffer::Pinned;
    // bind both ways
    page.bindBuffer(newno);
    newbuf.bindPage(&pageManager, pageno);
    // load buffer
    pageManager.loadPageBuffer(pageno);

    // return general case
    newbuf.stateLock.unlock();
    evictableListLock.unlock();
}

void anarion::db::BufferManager::evict(anarion::db::bufferno_t bufferno) {
    // variable prepare
    Buffer &buffer = getBuffer(bufferno);
    if (buffer.state != Buffer::Evictable) {
        throw EvictingNonEvictable();
    }
    pageno_t pageno = buffer.pageno;
    PageManager *pageManager = buffer.pageManager;
    Page &page = pageManager->getPage(pageno);
    // execute
    if (page.isDirty) {
        pageManager->flushPageBuffer(pageno);
    }
    // promise of out
    evictableList.remove(bufferno);
    page.isPresent = false;
}

void anarion::db::BufferManager::unloadPage(anarion::db::PageManager &pageManager, anarion::db::pageno_t pageno) {
    Page &page = pageManager.getPage(pageno);
    Buffer &buffer = getBuffer(page.bufferno);
    evictableListLock.lock();
    buffer.stateLock.lock();
    evictableList.add(page.bufferno);
    buffer.state = Buffer::Evictable;
    buffer.stateLock.unlock();
    evictableListLock.unlock();
    evictableCond.signal();
}
namespace anarion {
    class BufferLoggerInfo : public LoggerInfo {
    protected:
        db::BufferManager *bufferManager;
    public:
        BufferLoggerInfo(db::BufferManager *bufferManager1) : bufferManager(bufferManager1) {}
        void toChannel(InChannel &inChannel) override {
            SString info { SString::format("[Buffer manager Info] individual size[%lu]  total[%lu]  free[%lu] evictable[%lu] *",
                                           bufferManager->getBufferSize(), bufferManager->getTotalCount(),
                                           bufferManager->getFreeCount(), bufferManager->getEvictableCount()
                                           ) };
            inChannel.in(info.getArr(), info.length());
        }
    };

}

void anarion::db::BufferManager::logInfo(anarion::Logger &logger) {
    logger.addInfo(new BufferLoggerInfo(this));
}
