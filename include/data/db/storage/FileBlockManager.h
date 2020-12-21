//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_FILEBLOCKMANAGER_H
#define ANBASE_FILEBLOCKMANAGER_H

#include <io/channel/file/FileChannel.h>
#include <container/List/BitArray.h>
#include "typedecl.h"

namespace anarion {
namespace db {

class FileBlockManager {
public:
    static const blockno_t null = -1;
protected:
    struct BlockInfo {
        FileChannel *file;
        BitArray usedMap;
        blockoff_t minUnusedIndex;

        BlockInfo(FileChannel *file, blockoff_t blockSize, blockoff_t pageSize) : file(file), usedMap(blockSize / pageSize), minUnusedIndex(0) {}

        bool allocate(blockoff_t &pageIndex);
        void deallocate(blockoff_t pageIndex);
    };
    Vector<BlockInfo> blockFiles;
    blockoff_t blockSize, pageSize;
    Path fileDir;

    blockno_t allocateNewBlockFile(const Path &dir);

    FileChannel *fetchBlockChannel(blockno_t index);

public:
    explicit FileBlockManager(const Path &dir, size_type blockSize, blockoff_t pageSize, blockno_t blockInitCount = 1);
    ~FileBlockManager();

    constexpr size_type getBlockSize() const { return blockSize; }
    constexpr blockoff_t getPageSize() const { return pageSize; }
    constexpr blockno_t getBlockCount() const { return blockFiles.size(); }
    constexpr size_type getTotalSize() const { return getBlockSize() * blockFiles.size(); }

    void out(blockno_t blockno, blockoff_t offset, char *buffer, size_type length);
    void in(blockno_t blockno, blockoff_t offset, const char *buffer, size_type length);

    void allocateFreePage(blockno_t &blockno, blockoff_t &blockoff);
    void deallocatePage(blockno_t blockno, blockoff_t blockoff);

    struct Exception : public std::exception {};
    struct OutOfIndex : public Exception {};
};


}
}

#endif //ANBASE_FILEBLOCKMANAGER_H
