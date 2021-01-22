//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_PAGEMANAGER_H
#define ANBASE_PAGEMANAGER_H

#include <serialize/Serializer.h>
#include <data/db/loginfo/DataBaseLogEntry.h>
#include "FileBlockManager.h"
#include "../IdNamed.h"

namespace anarion {
namespace db {
    class BufferManager;
    struct Page {
        static const pageno_t pagenoNull = -1;
        bufferno_t bufferno = pagenoNull;
        bool isPresent : 1;
        bool isDirty : 1;
        uint32_t refCount = 0;

        Mutex mutex;
        Cond<Mutex> cond;

        struct Header {

        } header;

        static const size_type headerLength = sizeof(struct Header);

        void flushHeader(char *buf);
        void loadHeader(char *buf);

        Page() : isPresent(false), isDirty(false), mutex(), cond(mutex) {}

        constexpr void bindBuffer(bufferno_t bufferno1) {
            bufferno = bufferno1;
        }

        void waitLoad();

    };

    class PageManager : public IdNamed<uint16_t> {
    friend class Page;
    protected:
        Vector<Page> pageList;
        FileBlockManager *fileBlockManager;
        BufferManager *bufferManager;
        blockno_t beginBlock;
        const pageoff_t pageSize;

        void pageno2blockoff(pageno_t pageno, blockno_t &blockno, blockoff_t &blockoff) const ;

    public:
        PageManager(FileBlockManager *fileBlockManager1, blockno_t beginBlock, BufferManager *bufferManager1, pageno_t pageCount, pageoff_t pageSize);

        pageoff_t getValidPageSize() const { return pageSize - Page::headerLength; }

        Page &getPage(pageno_t pageno) { return pageList.get(pageno); }
        const Page &getPage(pageno_t pageno) const { return pageList.get(pageno); }
        pageno_t getPageno(Page *page) const ;

        void loadPageBuffer(pageno_t pageno);
        void flushPageBuffer(pageno_t pageno);

        void load(pageno_t pageno);
        void release(pageno_t pageno);

        void rawRead(pageno_t pageno, pageoff_t pageoff, char *buf, pageoff_t length);
        void rawWrite(pageno_t pageno, pageoff_t pageoff, const char *buf, pageoff_t length);
        void loadReadRelease(pageno_t pageno, pageoff_t pageoff, char *buf, pageoff_t length);
        void loadWriteRelease(pageno_t pageno, pageoff_t pageoff, const char *buf, pageoff_t length);

    struct Exception : public std::exception {};
    struct ComponentsPageSizeInconsistent : public Exception {};
    struct FlushingNonPresentPage : public Exception {};
    struct AccessOutofPageRange : public Exception {};
    };
}
}



#endif //ANBASE_PAGEMANAGER_H
