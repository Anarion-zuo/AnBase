//
// Created by 左钰 on 2020/5/29.
//

#include "io/channel/file/MemoryMappedFile.h"
#include <sys/mman.h>
#include <io/base/io-exceptions.h>

anarion::MemoryMappedFile::MemoryMappedFile(const anarion::SString &dir) : channel(FileChannel::open(dir)) {
    phead = mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, channel.getFd(), 0);
    if (phead == MAP_FAILED) { throw OpenFdException(); }
}

anarion::MemoryMappedFile::MemoryMappedFile(anarion::MemoryMappedFile &&rhs) noexcept : channel(move(rhs.channel)), phead(rhs.phead) {
    rhs.phead = nullptr;
}

anarion::MemoryMappedFile::~MemoryMappedFile() {
    munmap(phead, 4096);
    channel.close();
}

void anarion::MemoryMappedFile::sync() {
    msync(phead, 4096, MS_ASYNC);
}
