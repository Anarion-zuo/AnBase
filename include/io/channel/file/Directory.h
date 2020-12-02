//
// Created by anarion on 4/6/20.
//

#ifndef MYCPPBASE_DIRECTORY_H
#define MYCPPBASE_DIRECTORY_H

#include <dirent.h>
#include <SystemException.h>
#include "FileEntry.h"

namespace anarion {
/*
 * A Directory is also a file.
 * The DIR struct holds a file descriptor inside for convenient operations, which is an extra utility for a directory as a file.
 * Therefore, this class inherits all from FileChannel, making part of the operations invalid, protecting directory structure.
 *
 * Other than the utilities provided by the FileChannel class, this class encapsulates operations under dirent.h as an iterator.
 */

    class Directory : public FileChannel {
    protected:
        DIR *dirp = nullptr;
        mutable struct dirent *dirEnt = nullptr;

        void moveNextEnt() const ;
    public:
        explicit Directory(const SString &name);
        Directory(Directory &&rhs) noexcept ;

        ~Directory() override ;

        // open close
        void open() override;
        void close() override;
        void create(perm_t perm) override ;

        // read/write
        size_type in(const char *data, size_type nbytes) override ;
        size_type in(Buffer &buffer) override ;
        size_type in(Buffer &buffer, size_type nbytes) override ;
        size_type out(char *p, size_type nbytes) override ;
        Buffer out(size_type nbytes) override ;
        Buffer out() override ;

        // change operating offset
        void moveForthCursor(size_type steps) override ;
        void moveBackCursor(size_type steps) override ;
        void setCursor(size_type index) override ;

        // sizes
        size_type size() const override ;

        // remove/move/rename
        void remove() override ;

        // iterates
        struct dirent *next() const ;
        void rewindIterate() const ;
        size_type currentIndex() const ;
        void setIndex(size_type index);
        struct dirent *curEnt() const ;

        // conversions
        anarion::FileChannel direntToFile(flag_t oflags) const ;
    };
}

#endif //MYCPPBASE_DIRECTORY_H
