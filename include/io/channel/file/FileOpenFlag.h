//
// Created by anarion on 11/13/20.
//

#ifndef ANBASE_FILEOPENFLAG_H
#define ANBASE_FILEOPENFLAG_H

#include <fcntl.h>

namespace anarion {
    class FileOpenFlag {
        using oflag_t = int;
        oflag_t oflags = 0;

    public:
        FileOpenFlag() = default;
        explicit FileOpenFlag(oflag_t oflag) : oflags(oflag) {}

        oflag_t getOpenFlag() const { return oflags; }
        void clear() { oflags = 0; }

        void setReadOnly() {
            oflags |= O_RDONLY;
        }

        void setWriteOnly() {
            oflags |= O_WRONLY;
        }

        void setReadWrite() {
            oflags |= O_RDWR;
        }

        /**
         * @details Open a file with flag O_APPEND makes the kernel place the cursor at the end of the file before each syscall write. Henceforth, 2 processes may not append to the same file with different believes in where the end is.
         */
        void setAppend() {
            oflags |= O_APPEND;
        }

        /**
         * @details Creates file if not exists. If the file exists, open it normally.
         */
        void setCreateIfNotExists() {
            oflags |= O_CREAT;
        }

        /**
         * @details Generate error if path does not refer to a directory.
         */
        void setDirectory() {
            oflags |= O_DIRECTORY;
        }

        /**
         * @details Generate error if trying to create a file with existing name.
         */
        void setErrorOnCreateFileExists() {
            oflags |= O_EXCL;
        }

        void setNoTTY() {
            oflags |= O_NOCTTY;
        }

        void setNoFollowSymLink() {
            oflags |= O_NOFOLLOW;
        }

        void setNonBlock() {
            oflags |= O_NONBLOCK;
        }

        /**
         * @details Make each syscall write to block until the whole write operation is complete, giving a complete write through.
         */
        void setSync() {
            oflags |= O_SYNC;
        }

        /**
         * @details Write through waiting. Don't wait for file attributes to be updated if they don't affect the ability to read the data just written.
         */
        void setDSync() {
            oflags |= O_DSYNC;
        }

        /**
         * @details Enable direct IO
         */
        void setDirect() {
            oflags |= O_DIRECT;
        }

        /**
         * @details Have each syscall read operation wait until any pending writes for the same protion of the file are complete.
         */
        void setRSync() {
            oflags |= O_RSYNC;
        }

        void setTruncate() {
            oflags |= O_TRUNC;
        }
    };
}

#endif //ANBASE_FILEOPENFLAG_H
