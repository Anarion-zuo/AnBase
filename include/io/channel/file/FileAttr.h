//
// Created by anarion on 11/12/20.
//

#ifndef ANBASE_FILEATTR_H
#define ANBASE_FILEATTR_H

#include <sys/stat.h>
#include <time/Time.h>

namespace anarion {
class FileAttr {
protected:
    struct stat handle;
public:
    explicit FileAttr(struct stat &handle) : handle(handle) {}

    const struct stat &getHandle() const { return handle; }

    // st_mode
    mode_t getFileMode() const { return handle.st_mode; }
    bool isRegularFile() const { return S_ISREG(getFileMode()); }
    bool isDirectory() const { return S_ISDIR(getFileMode()); }
    bool isCharacterSpecial() const { return S_ISCHR(getFileMode()); }
    bool isBlockSpecial() const { return S_ISBLK(getFileMode()); }
    bool isFifo() const { return S_ISFIFO(getFileMode()); }
    bool isLink() const { return S_ISLNK(getFileMode()); }
    bool isSocket() const { return S_ISSOCK(getFileMode()); }

    // st_ino
    ino_t getInodeNumber() const { return handle.st_ino; }
    // st_dev
    ino_t getDeviceNumber() const { return handle.st_dev; }
    // st_rdev
    dev_t getSpecialDeviceNumber() const { return handle.st_rdev; }
    // st_nlink
    nlink_t getLinkNumber() const { return handle.st_nlink; }
    // st_uid
    uid_t getUserId() const { return handle.st_uid; }
    // st_gid
    gid_t getGroupId() const { return handle.st_gid; }
    // st_size
    off_t getFileSize() const { return handle.st_size; }
    // st_blksize
    blksize_t getIoBlockSize() const { return handle.st_blksize; }
    // st_blocks
    blkcnt_t getBlockNumber() const { return handle.st_blocks; }

    // st_atim
    Time getLastAccessTime() const {
        #ifdef __linux__
        return Time(handle.st_atim);
        #elif __APPLE__
        return Time(statInfo.st_atimespec);
        #endif
    }
    // st_mtim
    Time getLastModificationTime() const {
#ifdef __linux__
        return Time(handle.st_mtim);
#elif __APPLE__
        return Time(statInfo.st_mtimespec);
#endif
    }
    // st_ctim
    Time getLastStatusChangeTime() const {
#ifdef __linux__
        return Time(handle.st_ctim);
#elif __APPLE__
        return Time(statInfo.st_ctimespec);
#endif
    }
};
}

#endif //ANBASE_FILEATTR_H
