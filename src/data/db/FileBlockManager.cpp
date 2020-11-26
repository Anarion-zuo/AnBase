//
// Created by anarion on 11/25/20.
//

#include "data/db/FileBlockManager.h"

anarion::db::FileBlockManager::FileBlockManager(anarion::size_type blockSize, blockno_t blockInitCount)
:    blockSize(blockSize), blockFiles(blockInitCount) {

}

void anarion::db::FileBlockManager::allocateNewBlockFile(const Path &dir) {
    size_type blockno = blockFiles.size();
    FileOpenFlag oflag;
    oflag.setReadWrite();
    oflag.setCreateIfNotExists();
    FileChannel *channel = new FileChannel(Path::combine(dir, Path(SString::parseDec(blockno))), oflag);
    blockFiles.push_back(channel);
    channel->open(S_IRWXU);
}

anarion::db::FileBlockManager::~FileBlockManager() {
    for (size_type index = 0; index < blockFiles.size(); ++index) {
        delete blockFiles.get(index);
    }
}

anarion::db::FileBlockManager::FileBlockManager(const anarion::Path &dir, anarion::size_type blockSize, blockno_t blockInitCount)
    : FileBlockManager(blockSize, blockInitCount) {
    for (size_type index = 0; index < blockInitCount; ++index) {
        allocateNewBlockFile(dir);
    }
}

anarion::FileChannel *anarion::db::FileBlockManager::fetchBlockChannel(blockno_t index) {
    return blockFiles.get(index);
}

void anarion::db::FileBlockManager::out(blockno_t blockno, blockoff_t offset, char *buffer, anarion::size_type length) {
    if (offset >= blockSize) {
        throw DataBlockOperationOutOfIndex();
    }
    if (offset + length > blockSize) {
        throw DataBlockOperationOutOfIndex();
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
        throw DataBlockOperationOutOfIndex();
    }
    if (offset + length > blockSize) {
        throw DataBlockOperationOutOfIndex();
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
