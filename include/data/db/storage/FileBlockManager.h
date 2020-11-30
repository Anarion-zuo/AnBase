//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_FILEBLOCKMANAGER_H
#define ANBASE_FILEBLOCKMANAGER_H

#include <io/channel/file/FileChannel.h>
#include "typedecl.h"

namespace anarion {
namespace db {

class FileBlockManager {
public:
    static const blockno_t null = -1;
protected:
    Vector<FileChannel *> blockFiles;
    size_type blockSize;

    void allocateNewBlockFile(const Path &dir);

    FileChannel *fetchBlockChannel(blockno_t index);

public:
    explicit FileBlockManager(size_type blockSize, blockno_t blockInitCount = 1);
    explicit FileBlockManager(const Path &dir, size_type blockSize, blockno_t blockInitCount = 1);
    ~FileBlockManager();

    constexpr size_type getBlockSize() const { return blockSize; }
    constexpr blockno_t getBlockCount() const { return blockFiles.size(); }
    constexpr size_type getTotalSize() const { return getBlockSize() * blockFiles.size(); }

    void out(blockno_t blockno, blockoff_t offset, char *buffer, size_type length);
    void in(blockno_t blockno, blockoff_t offset, const char *buffer, size_type length);
};

struct DataBlockManagerException : public std::exception {};
struct DataBlockOperationOutOfIndex : public DataBlockManagerException {};
}
}

#endif //ANBASE_FILEBLOCKMANAGER_H
