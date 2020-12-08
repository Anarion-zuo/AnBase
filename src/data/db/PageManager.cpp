//
// Created by anarion on 11/25/20.
//

#include <io/channel/terminal/TerminalPrintChannel.h>
#include "data/db/storage/PageManager.h"

anarion::db::PageManager::PageManager(FileBlockManager *blockManager, BlockInfo blockInfo, pageoff_t pageSize, pageno_t pageInitCount, bufferno_t bufferInitCount) :
        blockManager(blockManager),
        blockOffset(blockInfo),
        bufferManager(pageSize, bufferInitCount),
        pageSize(pageSize),
        pageInfos(pageInitCount, PageInfo())
{
    initBlockOffsets();
    logInfo(new DataBaseLogEntry(SString::format("PageManager initialized at address 0x%lx pageSize %u", this, this->pageSize), new DebugLogLevel));
}

anarion::db::PageManager::PageInfo &
anarion::db::PageManager::getPage(anarion::db::pageno_t pageno) {
    if (pageno >= pageInfos.size()) {
        throw PageIndexOutOfRange();
    }
    return pageInfos.get(pageno);
}

void anarion::db::PageManager::warmPage(anarion::db::pageno_t pageno) {
    getPage(pageno).load(this, pageno);
}

void anarion::db::PageManager::evictPage(anarion::db::pageno_t pageno) {
    logInfo(new DataBaseLogEntry(SString::format("PageManager evicted pageno %u", pageno), new DebugLogLevel));
    getPage(pageno).flush(this);
    getPage(pageno).isPresent = false;
    getPage(pageno).setNullBuffer(this);
}

void anarion::db::PageManager::initBlockOffsets() {
    BlockInfo blockInfo = blockOffset;
    for (pageno_t pageno = 0; pageno < getPageCount(); ++pageno) {
        getPage(pageno).blockInfo = blockInfo;
        blockInfo.blockoff += pageSize;
        if (blockInfo.blockoff > blockManager->getBlockSize()) {
            ++blockInfo.blockno;
            blockInfo.blockoff = 0;
        }
    }
}

void anarion::db::PageManager::read(
        anarion::db::pageno_t pageno, anarion::db::pageoff_t pageoff,
        char *buffer,anarion::size_type length)
{
    warmPage(pageno);
    memcpy(buffer, bufferManager.getBufferAddress(getPage(pageno).bufferno) + pageoff, length);
    bufferManager.endUsing(getPage(pageno).bufferno);
}

void anarion::db::PageManager::write(
        anarion::db::pageno_t pageno, anarion::db::pageoff_t pageoff,
        const char *buffer, anarion::size_type length)
{
    warmPage(pageno);
    getPage(pageno).isDirty = true;
    memcpy(bufferManager.getBufferAddress(getPage(pageno).bufferno) + pageoff, buffer, length);
    bufferManager.endUsing(getPage(pageno).bufferno);
}

void anarion::db::PageManager::flushAll() {
    for (size_type index = 0; index < getPageCount(); ++index) {
        getPage(index).flush(this);
    }
}

anarion::db::PageManager::~PageManager() {
    flushAll();
}

bool anarion::db::PageManager::offset2info(anarion::size_type offset, anarion::db::PageManager::BlockInfo &blockInfo,
                                           anarion::db::pageno_t &pageno, anarion::db::pageoff_t &pageoff) const {
    if (!offset2PageInfo(offset, pageno, pageoff)) {
        return false;
    }
    return offset2BlockInfo(offset, blockInfo);
}

bool anarion::db::PageManager::offset2PageInfo(anarion::size_type offset, anarion::db::pageno_t &pageno,
                                               anarion::db::pageoff_t &pageoff) const {
    pageno = offset / pageSize;
    if (pageno >= pageInfos.size()) {
        return false;
    }
    pageoff = offset % pageSize;
    return true;
}

bool anarion::db::PageManager::offset2BlockInfo(anarion::size_type offset,
                                                anarion::db::PageManager::BlockInfo &blockInfo) const {
    blockInfo.blockno = offset / blockManager->getBlockSize() + blockOffset.blockno;
    blockInfo.blockoff = offset % blockManager->getBlockSize() + blockOffset.blockoff;
    if (blockInfo.blockoff >= blockManager->getBlockSize()) {
        ++blockInfo.blockno;
        blockInfo.blockoff -= blockManager->getBlockSize();
    }
    if (blockInfo.isOutOfRange(*blockManager)) {
        return false;
    }
    return true;
}

void anarion::db::PageManager::read(anarion::size_type offset, char *buffer, anarion::size_type length) {
    pageno_t pageno;
    pageoff_t pageoff;
    offset2PageInfo(offset, pageno, pageoff);
    read(pageno, pageoff, buffer, length);
}

void anarion::db::PageManager::write(anarion::size_type offset, const char *buffer, anarion::size_type length) {
    pageno_t pageno;
    pageoff_t pageoff;
    offset2PageInfo(offset, pageno, pageoff);
    write(pageno, pageoff, buffer, length);
}

void anarion::db::PageManager::logInfo(anarion::db::DataBaseLogEntry *entry) {
    logSerializer.add(entry);
    TerminalPrintChannel printer;
    logSerializer.commit(printer);
}

void anarion::db::PageManager::PageInfo::load(PageManager *pageManager, pageno_t pageno) {
    if (hasBuffer(pageManager, pageno)) {

    } else {
        bufferno = pageManager->bufferManager.mapPage(pageManager, pageno);
    }
    if (!isPresent) {
        pageManager->blockManager->out(
                blockInfo.blockno, blockInfo.blockoff,
                pageManager->bufferManager.getBufferAddress(bufferno),
                pageManager->pageSize
        );
        isPresent = true;
        isDirty = false;
    }
}

void anarion::db::PageManager::PageInfo::flush(PageManager *pageManager) const {
    if (!isPresent) {
        return;
    }
    if (isDirty) {
        pageManager->blockManager->in(
                blockInfo.blockno, blockInfo.blockoff,
                pageManager->bufferManager.getBufferAddress(bufferno),
                pageManager->pageSize
                );
    }
}

void anarion::db::PageManager::PageInfo::setNullBuffer(PageManager *pageManager) {
    pageManager->bufferManager.setPageno(bufferno, pagenoNull);
    bufferno = BufferManager::null;
}

bool anarion::db::PageManager::PageInfo::hasBuffer(PageManager *pageManager, pageno_t pageno) const {
    if (nullBuffer()) {
        return false;
    }
    BufferManager::BufferInfo &buffer = pageManager->bufferManager.getBuffer(bufferno);
    return buffer.pageno == pageno && buffer.pageManager == pageManager;
}
