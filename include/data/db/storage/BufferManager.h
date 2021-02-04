//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_BUFFERMANAGER_H
#define ANBASE_BUFFERMANAGER_H

#include <container/List/Vector.hpp>
#include "container/numtype/FreeList.hpp"
#include <container/numtype/NumListSet.h>
#include <logger/Logger.h>
#include "typedecl.h"
#include "LruList.h"
#include "PageManager.h"

namespace anarion {
namespace db {

    class PageManager;

    struct Buffer {
        char *bytes = nullptr;
        // buffer state
        Mutex stateLock;
        // indicates where is this buffer
        enum State {
            Free = 0,
            Evictable,
            Pinned,
        };
        enum State state = Free;
        // page manager
        PageManager *pageManager = nullptr;
        pageno_t pageno = Page::pagenoNull;

        void allocateBytes(size_type count);
        void deallocateBytes(size_type count);

        void bindPage(PageManager *pageManager1, pageno_t pageno1);
//        bool validBind();
    };

    /**
     * @brief Manages buffers backing disk blocks.
     * @details
     */
    class BufferManager {
    protected:
        Vector<Buffer> bufferList;
        FreeList<bufferno_t> freeList;
        LruList<bufferno_t> evictableList;
//        NumListSet<bufferno_t> pinnedList;
        const bufferoff_t bufferSize;
        Mutex freeListLock, evictableListLock, pinnedListLock;

        bufferno_t popFree();
        bufferno_t popEvictable();
        void pushFree(bufferno_t bufferno);
        void pushEvictable(bufferno_t bufferno);

        void evict(bufferno_t bufferno);
    public:
        BufferManager(bufferoff_t pageSize, bufferno_t bufferCount);
        ~BufferManager();

        Buffer &getBuffer(bufferno_t bufferno) { return bufferList.get(bufferno); }
        const Buffer &getBuffer(bufferno_t bufferno) const { return bufferList.get(bufferno); }
        bufferoff_t getBufferSize() const { return bufferSize; }
        bufferno_t getBufferno(const Buffer *pbuffer) const { return pbuffer - bufferList.beginIterator(); }

        bufferno_t getTotalCount() const { return bufferList.size(); }
        bufferno_t getEvictableCount() const { return evictableList.size(); }
//        bufferno_t getPinnedCount() const { return pinnedList.size(); }
        bufferno_t getFreeCount() const { return freeList.freeCount(); }

        void loadPage(PageManager &pageManager, pageno_t pageno);
        void unloadPage(PageManager &pageManager, pageno_t pageno);

        void logInfo(Logger &logger);

    struct Exception : public std::exception {};
    struct FreeListExhausted : public Exception {};
    struct WholePoolExhausted : public Exception {};  // the whole pool is in use
    struct FreeHangingBuffer : public Exception {};   // a buffer in the free list points to a page
    struct EvictingNonEvictable : public Exception {};
    };

}
}

#endif //ANBASE_BUFFERMANAGER_H
