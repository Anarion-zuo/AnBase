//
// Created by anarion on 4/6/20.
//

#ifndef MYCPPBASE_DIRECTORY_H
#define MYCPPBASE_DIRECTORY_H

#include <dirent.h>
#include "FileEntry.h"

namespace anarion {
    class Directory : public virtual FileEntry {
    protected:
        DIR *handle;
    public:
        explicit Directory(SString &&name);
        Directory(Directory &&rhs) noexcept : FileEntry(forward<Directory>(rhs)), handle(rhs.handle) { rhs.handle = nullptr; }

        void release() override;
        void open() override;
    };
}

#endif //MYCPPBASE_DIRECTORY_H
