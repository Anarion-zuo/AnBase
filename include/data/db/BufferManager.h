//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_BUFFERMANAGER_H
#define ANBASE_BUFFERMANAGER_H

#include <container/List/Vector.hpp>
#include "FreeList.hpp"
#include "typedecl.h"
#include "LruList.h"

namespace anarion {
namespace db {

    class PageManager;

    class BufferManager {
    public:
        static const blockno_t null = -1;

        struct BufferInfo {
            char *head;
            bool pin : 1;
            pageno_t pageno;
        };
    protected:
        Vector<BufferInfo> bufferList;
        FreeList<bufferno_t> freeList;
        LruList<bufferno_t> evictableList;
        const bufferoff_t bufferSize;

        void allocateNewBuffer(bufferno_t count);
        void deallocateBuffer(bufferno_t bufferno);

        BufferInfo &getBuffer(bufferno_t bufferno);
        const BufferInfo &getBuffer(bufferno_t bufferno) const ;

        anarion::db::bufferno_t allocate(pageno_t pageno, PageManager *pageManager);
        bufferno_t pickEvict();

    public:
        constexpr bufferno_t getBufferCount() const { return bufferList.size(); }
        constexpr bufferoff_t getBufferSize() const { return bufferSize; }
        constexpr size_type getTotalSize() const { return getBufferCount() * getBufferSize(); }

        BufferManager(bufferoff_t bufferSize, bufferno_t bufferInitCount);

        char *getBufferAddress(bufferno_t bufferno);
        const char *getBufferAddress(bufferno_t bufferno) const ;
        pageno_t getBufferPageno(bufferno_t) const ;
        void setPageno(bufferno_t bufferno, pageno_t pageno);

        anarion::db::bufferno_t allocatePinned(anarion::db::pageno_t pageno, PageManager *pageManager);
        anarion::db::bufferno_t allocateUnpinned(anarion::db::pageno_t pageno, PageManager *pageManager);

        void pin(bufferno_t bufferno);
        void unpin(bufferno_t bufferno);
    };

}
template<> struct move_traits<db::BufferManager::BufferInfo> {
    using has_move_ctor = false_type;
};
}

#endif //ANBASE_BUFFERMANAGER_H
