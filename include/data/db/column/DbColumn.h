//
// Created by anarion on 12/9/20.
//

#ifndef ANBASE_DBCOLUMN_H
#define ANBASE_DBCOLUMN_H

#include <data/db/storage/PageManager.h>

namespace anarion {
namespace db {
/**
 *
 * @tparam T Special type for database storage. Must have the following static interface:
 * @interface pageoff_t storageSize()
 * @interface void store(const char *p)
 * @interface void fetch(char *p)
 */
template <typename T>
class DbFixedColumn {
protected:
    PageManager pageManager;
    using eoff_t = uint32_t;
    eoff_t ecount;

    size_type eindex2totalOffset(eoff_t eindex) const {
        return T::storageSize() * eindex;
    }

    void eindex2pageinfo(eoff_t eindex, pageno_t &pageno, pageoff_t &pageoff) const {
        size_type totalOffset = eindex2totalOffset(eindex);
        pageno = totalOffset / pageManager.getValidPageSize();
        pageoff = totalOffset % pageManager.getValidPageSize();
        pageoff = pageManager.getValidPageSize() - pageoff;
    }

    eoff_t elementsPerPage() const {
        return pageManager.getValidPageSize() / T::storageSize();
    }
public:

    struct Exception : public std::exception {};
    struct IndexOutOfRange : public Exception {};

    DbFixedColumn(FileBlockManager *blockManager, pageoff_t pageSize, pageno_t pageInitCount, BufferManager *bufferManager)
    :
    pageManager(blockManager, 0, bufferManager, pageSize, pageInitCount)
    {}

    void loadPage(pageno_t pageno) {
        pageManager.load(pageno);
    }
    void releasePage(pageno_t pageno) {
        pageManager.release(pageno);
    }
    void loadPages(pageno_t begin, pageno_t end) {
        for (size_type pageno = begin; pageno < end; ++pageno) {
            loadPage(pageno);
        }
    }
    void releasePages(pageno_t begin, pageno_t end) {
        for (size_type pageno = begin; pageno < end; ++pageno) {
            releasePage(pageno);
        }
    }
    void loadElements(eoff_t begin, eoff_t count) {
        if (begin + count > ecount) {
            throw IndexOutOfRange();
        }
        // translate to pageno
        pageno_t beginno;
        pageoff_t beginoff;
        eindex2pageinfo(begin, beginno, beginoff);
        pageno_t endno;
        pageoff_t endoff;
        eindex2pageinfo(begin + count, endno, endoff);
        loadPages(beginno, endno);
        // check whether to load the last page
        if (endoff != 0) {
            loadPage(endno);
        }
    }
    bool isPresent(eoff_t index) {
        pageno_t beginno;
        pageoff_t beginoff;
        eindex2pageinfo(index, beginno, beginoff);
        return pageManager.getPage(beginno).isPresent;
    }

    T *read(eoff_t begin, eoff_t count) {

    }
};

}
}

#endif //ANBASE_DBCOLUMN_H
