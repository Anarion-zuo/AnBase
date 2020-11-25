//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_PAGEMANAGER_H
#define ANBASE_PAGEMANAGER_H

#include "FileBlockManager.h"
#include "BufferManager.h"

namespace anarion {
namespace db {

class PageManager {
protected:
    FileBlockManager *blockManager;
    BufferManager bufferManager;
    const pageoff_t pageSize;

    static const pageno_t pagenoNull = -1;

    struct PageInfo {
        static const bufferno_t buffernoNull = -1;
        static const blockno_t blocknoNull = -1;

        bufferno_t bufferno = buffernoNull;
        blockno_t blockno = blocknoNull;
        blockoff_t blockoff = 0;
        bool isDirty : 1;
        bool isPresent : 1;

        bool nullBuffer() const { return buffernoNull == bufferno; }
        bool nullBlock() const { return blocknoNull == blockno; }
        void setNullBuffer(PageManager *pageManager);
        bool hasBuffer(PageManager *pageManager, pageno_t pageno) const ;

        void load(PageManager *pageManager, pageno_t pageno);
        void flush(PageManager *pageManager);

        PageInfo() : isDirty(false), isPresent(false) {}
    };

    Vector<PageInfo> pageInfos;
    void initBlocks();

    PageInfo &getPage(pageno_t pageno);

    void warmPage(pageno_t pageno);
    void evictPage(pageno_t pageno);

public:
    constexpr pageno_t getPageCount() const { return pageInfos.size(); }
    constexpr size_type getTotalSize() const { return getPageCount() * pageSize; }
    constexpr pageoff_t getPageSize() const { return pageSize; }

    PageManager(FileBlockManager *blockManager, pageoff_t pageSize, pageno_t pageInitCount, bufferno_t bufferInitCount);

    void read(pageno_t pageno, pageoff_t pageoff, char *buffer, size_type length);
    void write(pageno_t pageno, pageoff_t pageoff, const char *buffer, size_type length);

struct Exception : public std::exception {};
struct PageNotBackedByBlock : public Exception {};
};

}
}

#endif //ANBASE_PAGEMANAGER_H
