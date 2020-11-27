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

public:
    static const pageno_t pagenoNull = -1;
protected:
    struct BlockInfo {
        blockno_t blockno;
        blockoff_t blockoff;

        BlockInfo(blockno_t blockno, blockoff_t blockoff)
            : blockno(blockno), blockoff(blockoff) {}
    };
    const BlockInfo blockOffset;

    struct PageInfo {
        bufferno_t bufferno = BufferManager::null;
        BlockInfo blockInfo { FileBlockManager::null, 0 };
        bool isDirty : 1;
        bool isPresent : 1;

        bool nullBuffer() const { return BufferManager::null == bufferno; }
        bool nullBlock() const { return FileBlockManager::null == blockInfo.blockno; }
        void setNullBuffer(PageManager *pageManager);
        bool hasBuffer(PageManager *pageManager, pageno_t pageno) const ;

        void load(PageManager *pageManager, pageno_t pageno);
        void flush(PageManager *pageManager);

        PageInfo() : isDirty(false), isPresent(false) {}
    };

    Vector<PageInfo> pageInfos;
    void initBlockOffsets();

    PageInfo &getPage(pageno_t pageno);

    void warmPage(pageno_t pageno);

public:
    constexpr pageno_t getPageCount() const { return pageInfos.size(); }
    constexpr size_type getTotalSize() const { return getPageCount() * pageSize; }
    constexpr pageoff_t getPageSize() const { return pageSize; }

    PageManager(FileBlockManager *blockManager, BlockInfo blockInfo, pageoff_t pageSize, pageno_t pageInitCount,
                bufferno_t bufferInitCount);
    ~PageManager();

    void read(pageno_t pageno, pageoff_t pageoff, char *buffer, size_type length);
    void write(pageno_t pageno, pageoff_t pageoff, const char *buffer, size_type length);
    void evictPage(pageno_t pageno);
    void flushAll();

struct Exception : public std::exception {};
struct PageNotBackedByBlock : public Exception {};
};

}
}

#endif //ANBASE_PAGEMANAGER_H