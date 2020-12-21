//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_PAGEMANAGER_H
#define ANBASE_PAGEMANAGER_H

#include <serialize/Serializer.h>
#include <data/db/loginfo/DataBaseLogEntry.h>
#include "FileBlockManager.h"
#include "BufferManager.h"
#include "../IdNamed.h"

namespace anarion {
namespace db {

class PageManager : public IdNamed<uint16_t> {
protected:
    FileBlockManager *blockManager;
    BufferManager *bufferManager;
    const pageoff_t pageSize;

public:
    static const pageno_t pagenoNull = -1;
protected:

    struct BlockInfo {
        blockno_t blockno;
        blockoff_t blockoff;

        BlockInfo(blockno_t blockno, blockoff_t blockoff) : blockno(blockno), blockoff(blockoff) {}

        size_type totalOffset(const FileBlockManager &blockManager) const {
            return blockManager.getBlockSize() * blockno + blockoff;
        }

        bool isOutOfRange(const FileBlockManager &blockManager) const {
            return totalOffset(blockManager) >= blockManager.getTotalSize();
        }
    };

    // meta data
    struct PageHeader {
        BlockInfo blockInfo;
        IdType managerId;
        pageno_t pageno;
    };

    struct PageManagerHeader {
        IdType managerId;
    };

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
        void flush(PageManager *pageManager) const;

        PageInfo() : isDirty(false), isPresent(false) {}

        void read(PageManager *pageManager, anarion::db::pageoff_t pageoff, char *buffer,
                  anarion::db::pageoff_t length);
        void write(PageManager *pageManager, pageoff_t pageoff, const char *buffer, pageoff_t length);

        PageHeader toHeader(PageManager *pageManager) const ;
        void writeMeta(PageManager *pageManager);
    };
    static pageoff_t pageMetaOffset() { return sizeof(PageHeader); }

    Vector<PageInfo> pageInfos;
    void initBlockOffsets();
    /*
    bool offset2info(size_type offset, BlockInfo &blockInfo, pageno_t &pageno, pageoff_t &pageoff) const ;
    bool offset2BlockInfo(size_type offset, BlockInfo &blockInfo) const ;
     */
    bool offset2PageInfo(size_type offset, pageno_t &pageno, pageoff_t &pageoff) const ;


    void warmPage(pageno_t pageno);

    // loginfo
    Serializer logSerializer;
    void logInfo(DataBaseLogEntry *entry);

public:

    constexpr pageno_t getPageCount() const { return pageInfos.size(); }
    constexpr size_type getTotalSize() const { return getPageCount() * pageSize; }
    pageoff_t getPageSize() const { return pageSize; }
    pageoff_t getValidPageSize() const { return pageSize - pageMetaOffset(); }

    PageInfo &getPage(pageno_t pageno);
    pageno_t pagenoOf(const PageInfo *page) const ;

    PageManager(FileBlockManager *blockManager, pageoff_t pageSize, pageno_t pageInitCount, BufferManager *bufferManager);
    ~PageManager();

    void singleAtomicRead(pageno_t pageno, pageoff_t pageoff, char *buffer, size_type length);
    void singleAtomicRead(size_type offset, char *buffer, size_type length);
    void singleAtomicWrite(pageno_t pageno, pageoff_t pageoff, const char *buffer, size_type length);
    void singleAtomicWrite(size_type offset, const char *buffer, size_type length);

    void evictPage(pageno_t pageno);
    void flushAll();

struct Exception : public std::exception {};
struct PageNotBackedByBlock : public Exception {};
struct PageIndexOutOfRange : public Exception {};
struct BufferPageSizeNotConsistent : public Exception {};
struct FileBlockPageSizeNotConsistent : public Exception {};

};

}
}

#endif //ANBASE_PAGEMANAGER_H
