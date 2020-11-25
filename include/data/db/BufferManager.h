//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_BUFFERMANAGER_H
#define ANBASE_BUFFERMANAGER_H

#include <container/List/Vector.hpp>
#include "FreeList.hpp"
#include "typedecl.h"

namespace anarion {
namespace db {

    class PageManager;

    class BufferManager {
    protected:

        struct BufferInfo {
            char *head;
            bool pin : 1;
            pageno_t pageno;
        };

        Vector<BufferInfo> bufferList;
        FreeList<bufferno_t> freeList;
        const bufferoff_t bufferSize;

        void allocateNewBuffer(bufferno_t count);
        void deallocateBuffer(bufferno_t bufferno);

        BufferInfo &getBuffer(bufferno_t bufferno);
        const BufferInfo &getBuffer(bufferno_t bufferno) const ;

        void evict(PageManager *pageManager);

    public:
        constexpr bufferno_t getBufferCount() const { return bufferList.size(); }
        constexpr bufferoff_t getBufferSize() const { return bufferSize; }
        constexpr size_type getTotalSize() const { return getBufferCount() * getBufferSize(); }

        BufferManager(bufferoff_t bufferSize, bufferno_t bufferInitCount);

        char *getBufferAddress(bufferno_t bufferno);
        const char *getBufferAddress(bufferno_t bufferno) const ;
        pageno_t getBufferPageno(bufferno_t) const ;
        void setPageno(bufferno_t bufferno, pageno_t pageno);

        bufferno_t getFree();
        bufferno_t getFreePin(pageno_t pageno);
    };
}
}

#endif //ANBASE_BUFFERMANAGER_H
