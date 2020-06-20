//
// Created by 左钰 on 2020/5/29.
//

#ifndef ANBASE_MEMORYMAPPEDFILE_H
#define ANBASE_MEMORYMAPPEDFILE_H

#include <container/SString.h>
#include "io/channel/file/FileChannel.h"

namespace anarion {
    class MemoryMappedFile {
    protected:
        FileChannel channel;
        void *phead;
    public:
        explicit MemoryMappedFile(const anarion::SString &dir);
        MemoryMappedFile(MemoryMappedFile &&rhs) noexcept ;
        ~MemoryMappedFile();

        constexpr void *getPointer() const { return phead; }
        void sync();
        FileChannel &getChannel() { return channel; }
    };
}

#endif //ANBASE_MEMORYMAPPEDFILE_H
