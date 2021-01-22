//
// Created by anarion on 11/25/20.
//

#include "data/db/storage/PageManager.h"
#include <data/db/storage/BufferManager.h>

anarion::db::PageManager::PageManager(FileBlockManager *fileBlockManager1, blockno_t beginBlock, BufferManager *bufferManager1, pageno_t pageCount, pageoff_t pageSize) :
pageSize(pageSize), pageList(pageCount, Page()), bufferManager(bufferManager1), fileBlockManager(fileBlockManager1), beginBlock(beginBlock)
{
    if (bufferManager->getBufferSize() != pageSize || fileBlockManager->getPageSize() != pageSize) {
        throw ComponentsPageSizeInconsistent();
    }
}

void anarion::db::PageManager::pageno2blockoff(pageno_t pageno, blockno_t &blockno, blockoff_t &blockoff) const {
    size_type totalSize = pageno * pageSize;
    blockno = totalSize / fileBlockManager->getBlockSize() + beginBlock;
    blockoff = totalSize % fileBlockManager->getBlockSize();
}

anarion::db::pageno_t anarion::db::PageManager::getPageno(anarion::db::Page *page) const {
    return page - pageList.beginIterator();
}
void anarion::db::PageManager::loadPageBuffer(pageno_t pageno) {
    Page &page = getPage(pageno);
    blockno_t blockno;
    blockoff_t blockoff;
    pageno2blockoff(pageno, blockno, blockoff);
    fileBlockManager->out(blockno, blockoff, bufferManager->getBuffer(page.bufferno).bytes, pageSize);
    page.isPresent = true;
    page.isDirty = false;
}

void anarion::db::PageManager::flushPageBuffer(pageno_t pageno) {
    Page &page = getPage(pageno);
    if (!page.isPresent) {
        throw FlushingNonPresentPage();
    }
    blockno_t blockno;
    blockoff_t blockoff;
    pageno2blockoff(pageno, blockno, blockoff);
    fileBlockManager->in(blockno, blockoff, bufferManager->getBuffer(page.bufferno).bytes, pageSize);
    page.isDirty = false;
}

void anarion::db::PageManager::load(pageno_t pageno) {
    Page &page = getPage(pageno);
    if (page.isPresent) {
        ++page.refCount;
        return;
    }
    bufferManager->loadPage(*this, pageno);
    page.isPresent = true;
    ++page.refCount;
    page.cond.broadcast();
}

void anarion::db::PageManager::release(pageno_t pageno) {
    Page &page = getPage(pageno);
    if (page.refCount == 0) {
        return;
    }
    --page.refCount;
    if (page.refCount == 0) {
        bufferManager->unloadPage(*this, pageno);
    }
}

void anarion::db::PageManager::loadReadRelease(pageno_t pageno, pageoff_t pageoff, char *buf, pageoff_t length) {
    if (pageoff + length > pageSize) {
        throw AccessOutofPageRange();
    }
    load(pageno);
    rawRead(pageno, pageoff, buf, length);
    release(pageno);
}

void anarion::db::PageManager::loadWriteRelease(pageno_t pageno, pageoff_t pageoff, const char *buf, pageoff_t length) {
    if (pageoff + length > pageSize) {
        throw AccessOutofPageRange();
    }
    load(pageno);
    rawWrite(pageno, pageoff, buf, length);
    release(pageno);
}

void anarion::db::PageManager::rawRead(pageno_t pageno, pageoff_t pageoff, char *buf, pageoff_t length) {
    Page &page = getPage(pageno);
    memcpy(buf, bufferManager->getBuffer(page.bufferno).bytes + pageoff, length);
}

void anarion::db::PageManager::rawWrite(pageno_t pageno, pageoff_t pageoff, const char *buf, pageoff_t length) {
    Page &page = getPage(pageno);
    memcpy(bufferManager->getBuffer(page.bufferno).bytes + pageoff, buf, length);
    page.isDirty = true;
}

void anarion::db::Page::loadHeader(char *buf) {
    memcpy(&this->header, buf, sizeof(struct Header));
}

void anarion::db::Page::flushHeader(char *buf) {
    memcpy(buf, &this->header, sizeof(struct Header));
}

void anarion::db::Page::waitLoad() {
    mutex.lock();
    while (refCount == 0) {
        cond.waitRaw();
    }
    mutex.unlock();
}
