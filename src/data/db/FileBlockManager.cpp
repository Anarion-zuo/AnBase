//
// Created by anarion on 11/25/20.
//

#include "data/db/storage/FileBlockManager.h"

anarion::db::blockno_t anarion::db::FileBlockManager::allocateNewBlockFile(const Path &dir) {
    size_type blockno = blockFiles.size();
    FileOpenFlag oflag;
    oflag.setReadWrite();
    oflag.setCreateIfNotExists();
//    oflag.setDirect();
    FileChannel *channel = new FileChannel(Path::combine(dir, Path(SString::parseDec(blockno))), oflag);
    blockFiles.pushBack(BlockInfo(channel, blockSize, pageSize));
    channel->open(S_IRWXU);
    return blockno;
}

anarion::db::FileBlockManager::~FileBlockManager() {
    for (size_type index = 0; index < blockFiles.size(); ++index) {
        delete blockFiles.get(index).file;
    }
}

anarion::db::FileBlockManager::FileBlockManager(const anarion::Path &dir, anarion::size_type blockSize, blockoff_t pageSize, blockno_t blockInitCount)
    : blockSize(blockSize), blockFiles(blockInitCount), pageSize(pageSize), fileDir(dir) {
    for (size_type index = 0; index < blockInitCount; ++index) {
        allocateNewBlockFile(dir);
    }
}

anarion::FileChannel *anarion::db::FileBlockManager::fetchBlockChannel(blockno_t index) {
    return blockFiles.get(index).file;
}

void anarion::db::FileBlockManager::out(blockno_t blockno, blockoff_t offset, char *buffer, anarion::size_type length) {
    if (offset >= blockSize) {
        throw OutOfIndex();
    }
    if (offset + length > blockSize) {
        throw OutOfIndex();
    }
    if (buffer == nullptr) {
        return;
    }
    if (length == 0) {
        return;
    }
    FileChannel *file = fetchBlockChannel(blockno);
    file->setCursor(offset);
    file->out(buffer, length);
}

void anarion::db::FileBlockManager::in(blockno_t blockno, blockoff_t offset, const char *buffer, anarion::size_type length) {
    if (offset >= blockSize) {
        throw OutOfIndex();
    }
    if (offset + length > blockSize) {
        throw OutOfIndex();
    }
    if (buffer == nullptr) {
        return;
    }
    if (length == 0) {
        return;
    }
    FileChannel *file = fetchBlockChannel(blockno);
    file->setCursor(offset);
    file->in(buffer, length);
}

void
anarion::db::FileBlockManager::allocateFreePage(anarion::db::blockno_t &blockno, anarion::db::blockoff_t &blockoff) {
    blockoff_t pageIndex;
    for (blockno_t index = 0; index < blockFiles.size(); ++index) {
        BlockInfo &block = blockFiles.get(index);
        if (block.allocate(pageIndex)) {
            // free page obtained
            blockno = index;
            blockoff = pageIndex * pageSize;
            return;
        } else {
            // must look at another block
        }
    }
    // must allocate a new block
    blockno = allocateNewBlockFile(fileDir);
    blockFiles.get(blockno).allocate(pageIndex);
    blockoff = pageIndex * pageSize;
}

void anarion::db::FileBlockManager::deallocatePage(anarion::db::blockno_t blockno, anarion::db::blockoff_t blockoff) {
    blockFiles.get(blockno).deallocate(blockoff / pageSize);
}

bool anarion::db::FileBlockManager::BlockInfo::allocate(anarion::db::blockoff_t &pageIndex) {
    blockoff_t ret = minUnusedIndex;
    if (ret >= usedMap.getLength()) {
        return false;
    }
    usedMap.set(ret);
    pageIndex = ret;
    while (true) {
        ++minUnusedIndex;
        if (minUnusedIndex >= usedMap.getLength()) {
            break;
        }
        if (!usedMap.check(minUnusedIndex)) {
            break;
        }
    }
    return true;
}

void anarion::db::FileBlockManager::BlockInfo::deallocate(anarion::db::blockoff_t pageIndex) {
    usedMap.clear(pageIndex);
    if (pageIndex < minUnusedIndex) {
        minUnusedIndex = pageIndex;
    }
}
