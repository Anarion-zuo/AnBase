//
// Created by anarion on 4/6/20.
//

#ifndef MYCPPBASE_DIRECTORY_H
#define MYCPPBASE_DIRECTORY_H

#include <dirent.h>
#include <SystemException.h>
#include "FileEntry.h"

namespace anarion {

    struct DirectoryException : public SystemException {};
    struct DirectoryCreateException : public DirectoryException {};
    struct RemoveFileException : public DirectoryCreateException {};

    class Directory : public FileEntry {
    protected:
        DIR *handle;
    public:
        explicit Directory(SString &&name);
        Directory(SString &&name, FileEntry *entry);
        Directory(Directory &&rhs) noexcept : FileEntry(forward<Directory>(rhs)), handle(rhs.handle) { rhs.handle = nullptr; }

        ~Directory() override ;

        void release() override;
        void open() override;
        void remove() override ;
        void close() override ;

        FileEntry *createChildFile(const SString &fileName);
        FileEntry *createChildDirectory(SString &&dirName);
    };
}

#endif //MYCPPBASE_DIRECTORY_H
