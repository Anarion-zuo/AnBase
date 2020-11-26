#include "container/SString.h"
#include <exceptions/InvalidOperation.h>
#include <io/channel/file/FileChannel.h>
#include <sys/stat.h>

void anarion::FileChannel::create(mode_t mode) {
    close();
    int ret = ::creat(path.getString().cstr(), mode);
    if (ret < 0) {
        throw CreatFailed();
    }
//    fetchAttributes();
}

void anarion::FileChannel::close() {
    if (isOpen()) {
        ::close(fd);
        fd = -1;
    }
}

/*
void anarion::FileChannel::move(const anarion::SString &newPath) {
    int ret = ::rename(path.getString().cstr(), newPath.cstr());
    if (ret < 0) {
        throw RenameFailed();
    }
    path = Path(newPath);
}*/
/*
void anarion::FileChannel::rename(const anarion::SString &newName) {
    Path newPath = Path::combine(path.getParent(), Path(newName));
    int ret = ::rename(path.getString().cstr(), newPath.getString().cstr());
    if (ret < 0) {
        throw RenameFailed();
    }
    path = anarion::move(newPath);
}*/

bool anarion::FileChannel::isOpen() const {
    return fd >= 0;
}

#pragma region offset
void anarion::FileChannel::resetCursor() {
    if (!isOpen()) { throw InvalidOperation(); }
    ::lseek(fd, 0, SEEK_SET);
}

void anarion::FileChannel::setCursorAppend() {
    if (!isOpen()) { throw InvalidOperation(); }
    ::lseek(fd, 0, SEEK_END);
}

void anarion::FileChannel::moveForthCursor(size_type nbytes) {
    if (!isOpen()) { throw InvalidOperation(); }
    ::lseek(fd, nbytes, SEEK_CUR);
}

void anarion::FileChannel::moveBackCursor(size_type nbytes) {
    if (!isOpen()) { throw InvalidOperation(); }
    ::lseek(fd, -nbytes, SEEK_CUR);
}

void anarion::FileChannel::setCursor(anarion::size_type index) {
    ::lseek(fd, index, SEEK_SET);
}

anarion::size_type anarion::FileChannel::size() const {
    checkIsOpen();
    off_t off = ::lseek(fd, 0, SEEK_CUR);
    size_type ret = ::lseek(fd, 0, SEEK_END);
    ::lseek(fd, off, SEEK_SET);
    return ret;
}
#pragma endregion

#pragma region read_write
anarion::size_type anarion::FileChannel::in(const char *p, size_type nbytes) {
    checkIsOpen();
    size_type ret = writen(fd, (void *) p, nbytes);
    return ret;
}

anarion::size_type anarion::FileChannel::out(char *p, size_type nbytes) {
    checkIsOpen();
    return readn(fd, p, nbytes);
}

#pragma endregion

void anarion::FileChannel::open() {
    close();
    fd = ::open(path.getString().cstr(), oflags.getOpenFlag());
    if (fd < 0) {
        throw OpenFdFailed();
    }
}

void anarion::FileChannel::open(mode_t mode) {
    close();
    fd = ::open(path.getString().cstr(), oflags.getOpenFlag(), mode);
    if (fd < 0) {
        throw OpenFdFailed();
    }
}
/*
void anarion::FileChannel::remove() {
    close();
    int ret = ::remove(path.getString().cstr());
    if (ret < 0) {
        throw RemoveFileFailed();
    }
}*/

anarion::FileChannel::FileChannel(anarion::Path &&path, flag_t oflags)
    : path(forward<Path>(path)), oflags(oflags) {

}

anarion::FileChannel::FileChannel(const anarion::SString &dir, int oflag)
    : path(SString(dir)), oflags(oflag) {
}

anarion::FileChannel::~FileChannel() {
    if (fd >= 0) {
        /*
         * Must not call virtual functions here,
         * therefore check manually.
         */
        ::close(fd);
        fd = -1;
    }
}

void anarion::FileChannel::sync() {
    checkIsOpen();
    int ret = fsync(fd);
    if (ret == -1) {
        throw FsyncFailed();
    }
}
/*
void anarion::FileChannel::symlink(const anarion::SString &linkPath) {
    int ret = ::symlink(path.getString().cstr(), linkPath.cstr());
    if (ret == -1) {
        throw LinkFailed();
    }
}*/
/*
void anarion::FileChannel::hardlink(const anarion::SString &linkPath) {
    int ret = link(path.getString().cstr(), linkPath.cstr());
    if (ret < 0) {
        throw LinkFailed();
    }
}*/
/*
void anarion::FileChannel::changePermission(int perm) {
    int ret = fchmod(fd, perm);
    if (ret < 0) {
        throw ChmodFailed();
    }
}

bool anarion::FileChannel::hasPermission(anarion::FileEntry::perm_t perm) {
    return perm & statInfo.st_mode;
}*/
/*
void anarion::FileChannel::fetchAttributes() {
    int ret = ::fstat(fd, &statInfo);
    if (ret < 0) {
        throw StatFailed();
    }
}*/

anarion::FileAttr anarion::FileChannel::getAttributes() const {
    struct stat stat;
    int ret = ::fstat(fd, &stat);
    if (ret < 0) {
        throw StatFailed();
    }
    return FileAttr(stat);
}
/*
anarion::Time anarion::FileChannel::getLastAccessTime() {
    fetchAttributes();
#ifdef __linux__
    return Time(statInfo.st_atim);
#elif __APPLE__
    return Time(statInfo.st_atimespec);
#endif
}

anarion::Time anarion::FileChannel::getLastModifyTime() {
    fetchAttributes();
#ifdef __linux__
    return Time(statInfo.st_mtim);
#elif __APPLE__
    return Time(statInfo.st_mtimespec);
#endif
}

anarion::Time anarion::FileChannel::getLastStatusChangeTime() {
    fetchAttributes();
#ifdef __linux__
    return Time(statInfo.st_ctim);
#elif __APPLE__
    return Time(statInfo.st_ctimespec);
#endif
}*/

anarion::FileChannel::FileChannel(anarion::FileChannel &&rhs) noexcept
    : path(move(rhs.path)), oflags(rhs.oflags), fd(rhs.fd) {
    rhs.fd = -1;
    rhs.oflags.clear();
}

void anarion::FileChannel::checkIsOpen() const {
    if (!isOpen()) {
        throw FileOperationOnNotOpened();
    }
}

int anarion::FileChannel::callFcntl(int cmd) const {
    int ret = fcntl(fd, cmd);
    if (ret < 0) {
        throw FcntlException();
    }
    return ret;
}

int anarion::FileChannel::callFcntl(int cmd, int arg) const {
    int ret = fcntl(fd, cmd, arg);
    if (ret < 0) {
        throw FcntlException();
    }
    return ret;
}

anarion::FileChannel::FileChannel(anarion::Path &&path, anarion::FileOpenFlag oflags)
    : path(forward<Path>(path)), oflags(oflags) {

}
