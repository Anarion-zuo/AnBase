//
// Created by anarion on 11/25/20.
//

#include "data/db/PageManager.h"

anarion::db::PageManager::PageManager(FileBlockManager *blockManager, BlockInfo blockInfo, pageoff_t pageSize, pageno_t pageInitCount, bufferno_t bufferInitCount) :
        blockManager(blockManager),
        blockOffset(blockInfo),
        bufferManager(pageSize, bufferInitCount),
        pageSize(pageSize),
        pageInfos(pageInitCount, PageInfo())
{
    initBlockOffsets();
}

anarion::db::PageManager::PageInfo &
anarion::db::PageManager::getPage(anarion::db::pageno_t pageno) {
    return pageInfos.get(pageno);
}

void anarion::db::PageManager::warmPage(anarion::db::pageno_t pageno) {
    getPage(pageno).load(this, pageno);
}

void anarion::db::PageManager::evictPage(anarion::db::pageno_t pageno) {
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
    if (length + pageoff > pageSize) {
        throw IndexOutOfRange();
    }
    memcpy(buffer, bufferManager.getBufferAddress(getPage(pageno).bufferno) + pageoff, length);
    bufferManager.unpin(getPage(pageno).bufferno);
}

void anarion::db::PageManager::write(
        anarion::db::pageno_t pageno, anarion::db::pageoff_t pageoff,
        const char *buffer, anarion::size_type length)
{
    warmPage(pageno);
    if (length + pageoff > pageSize) {
        throw IndexOutOfRange();
    }
    getPage(pageno).isDirty = true;
    memcpy(bufferManager.getBufferAddress(getPage(pageno).bufferno) + pageoff, buffer, length);
    bufferManager.unpin(getPage(pageno).bufferno);
}

void anarion::db::PageManager::flushAll() {
    for (size_type index = 0; index < getPageCount(); ++index) {
        getPage(index).flush(this);
    }
}

anarion::db::PageManager::~PageManager() {
    flushAll();
}

void anarion::db::PageManager::PageInfo::load(PageManager *pageManager, pageno_t pageno) {
    if (hasBuffer(pageManager, pageno)) {

    } else {
        bufferno = pageManager->bufferManager.allocatePinned(pageno, nullptr);
//    pageManager->bufferManager.setPageno(bufferno, pageno);
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

void anarion::db::PageManager::PageInfo::flush(PageManager *pageManager) {
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
    return pageManager->bufferManager.getBufferPageno(bufferno) == pageno;
}
