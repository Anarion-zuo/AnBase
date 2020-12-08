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

    /**
     * @brief Manages buffers backing disk blocks.
     * @details
     */
    class BufferManager {
    public:
        static const blockno_t null = -1;

        struct BufferInfo {
            char *head;
            size_type refCount;
            pageno_t pageno;
            PageManager *pageManager;
        };
    protected:
        Vector<BufferInfo> bufferList;
        FreeList<bufferno_t> freeList;
        LruList<bufferno_t> evictableList;
        const bufferoff_t bufferSize;

        void allocateNewBuffer(bufferno_t count);
        void deallocateBuffer(bufferno_t bufferno);

        bufferno_t pickEvict();

        /**
         * @brief Creates mapping between certain page and buffer.
         * @param pageManager Pointer to PageManager object.
         * @param pageno Index of the page in the page manager obejct.
         * @param bufferno Index of the buffer in this buffer manager.
         */
        void map(PageManager *pageManager, pageno_t pageno, bufferno_t bufferno);
        /**
         * @brief Destroy mapping beween certain page and buffer.
         * @param bufferno Index of the buffer in this buffer manager.
         */
        void unmap(bufferno_t bufferno);
        void markUsing(bufferno_t bufferno);
        void markNotUsing(bufferno_t bufferno);
        bufferno_t allocateOne();
        void evict(bufferno_t bufferno);

    public:
        constexpr bufferno_t getBufferCount() const { return bufferList.size(); }
        constexpr bufferoff_t getBufferSize() const { return bufferSize; }
        constexpr size_type getTotalSize() const { return getBufferCount() * getBufferSize(); }

        BufferInfo &getBuffer(bufferno_t bufferno);
        const BufferInfo &getBuffer(bufferno_t bufferno) const ;

        BufferManager(bufferoff_t bufferSize, bufferno_t bufferInitCount);

        char *getBufferAddress(bufferno_t bufferno);
        const char *getBufferAddress(bufferno_t bufferno) const ;
        pageno_t getBufferPageno(bufferno_t) const ;
        void setPageno(bufferno_t bufferno, pageno_t pageno);

        void beginUsing(bufferno_t bufferno);
        void endUsing(bufferno_t bufferno);
        bufferno_t mapPage(PageManager *pageManager, pageno_t pageno);

    struct Exception : public std::exception {};
    struct EvictingPinnedPage : public Exception {};
    };

}
template<> struct move_traits<db::BufferManager::BufferInfo> {
    using has_move_ctor = false_type;
};
}

#endif //ANBASE_BUFFERMANAGER_H
