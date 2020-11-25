//
// Created by anarion on 11/25/20.
//

#include "data/db/PageManager.h"

anarion::db::PageManager::PageManager(
        anarion::db::FileBlockManager *blockManager,
        anarion::db::pageoff_t pageSize,
        anarion::db::pageno_t pageInitCount,
        anarion::db::bufferno_t bufferInitCount
        ) :
        blockManager(blockManager),
        bufferManager(pageSize, bufferInitCount),
        pageSize(pageSize),
        pageInfos(pageInitCount, PageInfo())
{
    initBlocks();
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

void anarion::db::PageManager::initBlocks() {

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
}

void anarion::db::PageManager::PageInfo::load(PageManager *pageManager, pageno_t pageno) {
    if (hasBuffer(pageManager, pageno)) {

    } else {
        bufferno = pageManager->bufferManager.getFreePin(pageno);
//    pageManager->bufferManager.setPageno(bufferno, pageno);
    }
    if (!isPresent) {
        pageManager->blockManager->out(
                blockno, blockoff,
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
                blockno, blockoff,
                pageManager->bufferManager.getBufferAddress(bufferno),
                pageManager->pageSize
                );
    }
}

void anarion::db::PageManager::PageInfo::setNullBuffer(PageManager *pageManager) {
    pageManager->bufferManager.setPageno(bufferno, pagenoNull);
    bufferno = buffernoNull;
}

bool anarion::db::PageManager::PageInfo::hasBuffer(PageManager *pageManager, pageno_t pageno) const {
    if (nullBuffer()) {
        return false;
    }
    return pageManager->bufferManager.getBufferPageno(bufferno) == pageno;
}
