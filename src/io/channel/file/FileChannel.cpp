#include "container/SString.h"
#include <exceptions/InvalidOperation.h>
#include <io/channel/file/FileChannel.h>
#include <sys/stat.h>

anarion::FileChannel anarion::FileChannel::open(const SString &dir) {
    char *cdir = dir.copy_cstr();
    int fd = ::open(cdir, O_RDWR | O_CREAT, 0666);
    dir.release_copied(cdir);
    if (fd < 0) { throw OpenFdFailed(); }
    return FileChannel(SString(dir), fd);
}

void anarion::FileChannel::create(perm_t perm) {
    int ret = ::creat(path.getString().cstr(), perm);
    if (ret < 0) {
        throw OpenFdFailed();
    }
    fetchAttributes();
}

void anarion::FileChannel::close() {
    if (isOpen()) {
        ::close(fd);
        fd = -1;
    }
}
void anarion::FileChannel::move(const anarion::SString &newPath) {
    int ret = ::rename(path.getString().cstr(), newPath.cstr());
    if (ret < 0) {
        throw RenameFailed();
    }
    path = Path(newPath);
}

void anarion::FileChannel::rename(const anarion::SString &newName) {
    Path newPath = Path::combine(path.getParent(), Path(newName));
    int ret = ::rename(path.getString().cstr(), newPath.getString().cstr());
    if (ret < 0) {
        throw RenameFailed();
    }
    path = anarion::move(newPath);
}

bool anarion::FileChannel::isOpen() const {
    return fd > 0;
}

#pragma region offset
void anarion::FileChannel::rewind() {
    if (fd < 0) { throw InvalidOperation(); }
    ::lseek(fd, 0, SEEK_SET);
}

void anarion::FileChannel::set_append() {
    if (fd < 0) { throw InvalidOperation(); }
    ::lseek(fd, 0, SEEK_END);
}

void anarion::FileChannel::move_forth(size_type nbytes) {
    if (fd >= 0) { throw InvalidOperation(); }
    ::lseek(fd, nbytes, SEEK_CUR);
}

void anarion::FileChannel::move_back(size_type nbytes) {
    if (fd >= 0) { throw InvalidOperation(); }
    ::lseek(fd, -nbytes, SEEK_CUR);
}

void anarion::FileChannel::set_cursor(anarion::size_type index) {
    ::lseek(fd, index, SEEK_SET);
}

anarion::size_type anarion::FileChannel::size() const {
    if (isOpen()) { throwInvalidOperation(); }
    off_t off = ::lseek(fd, 0, SEEK_CUR);
    size_type ret = ::lseek(fd, 0, SEEK_END);
    ::lseek(fd, off, SEEK_SET);
    return ret;
}
#pragma endregion

#pragma region read_write
anarion::size_type anarion::FileChannel::in(const char *p, size_type nbytes) {
    if (!isOpen()) { throwInvalidOperation(); }
    size_type ret = writen(fd, (void *) p, nbytes);
    return ret;
}

anarion::size_type anarion::FileChannel::in(Buffer &buffer) {
    if (!isOpen()) { throwInvalidOperation(); }
    size_type ret = buffer.write_fd(fd, buffer.unread());
    return ret;
}

anarion::size_type anarion::FileChannel::in(Buffer &buffer, size_type nbytes) {
    if (!isOpen()) { throwInvalidOperation(); }
    if (nbytes > buffer.unread()) { throw IndexOutOfRange(); }
    size_type ret = buffer.write_fd(fd, nbytes);
    return ret;
}

anarion::size_type anarion::FileChannel::out(char *p, size_type nbytes) {
    if (!isOpen()) { throwInvalidOperation(); }
    return readn(fd, p, nbytes);
}

anarion::Buffer anarion::FileChannel::out(size_type nbytes) {
    if (!isOpen()) { throwInvalidOperation(); }
    Buffer buffer(nbytes);
    buffer.append_fd(fd, nbytes);
    return anarion::move(buffer);
}

anarion::Buffer anarion::FileChannel::out() {
    if (!isOpen()) { throwInvalidOperation(); }
    off_t cur = ::lseek(fd, 0, SEEK_CUR);
    off_t size = ::lseek(fd, 0, SEEK_END);
    ::lseek(fd, cur, SEEK_SET);
    off_t unread = size - cur;
    return out(unread);
}
#pragma endregion

void anarion::FileChannel::open() {
    fd = ::open(path.getString().cstr(), oflags);
    if (fd < 0) {
        throw OpenFdFailed();
    }
    ::fstat(fd, &statInfo);
}

void anarion::FileChannel::open(anarion::FileEntry::perm_t perm) {
    fd = ::open(path.getString().cstr(), oflags, perm);
    if (fd < 0) {
        throw OpenFdFailed();
    }
    ::fstat(fd, &statInfo);
}

void anarion::FileChannel::remove() {
    close();
    int ret = ::remove(path.getString().cstr());
    if (ret < 0) {
        throw RemoveFileFailed();
    }
}

anarion::FileChannel::FileChannel(const anarion::SString &dir)
    : FileEntry(SString(dir)), oflags(O_RDWR | O_CREAT) {
}

anarion::FileChannel::FileChannel(anarion::Path &&path, flag_t oflags)
    : FileEntry(forward<Path>(path)), oflags(oflags) {

}

anarion::FileChannel::FileChannel(const anarion::SString &dir, int oflag)
    : FileEntry(SString(dir)), oflags(oflag) {
}

anarion::FileChannel::FileChannel(const anarion::SString &dir, int oflag, int perm)
    : FileEntry(SString(dir)), oflags(oflag) {
}

anarion::FileChannel::FileChannel(anarion::FileChannel &&rhs) noexcept :
    InChannel(forward<FileChannel>(rhs)), OutChannel(forward<FileChannel>(rhs)), RandomChannel(forward<FileChannel>(rhs)), FileEntry(forward<FileChannel>(rhs)) , fd(rhs.fd), oflags(rhs.oflags) {
    rhs.fd = -1;
    rhs.oflags = 0;
}

anarion::FileChannel::FileChannel(anarion::SString &&name, int fd, flag_t oflags) : FileEntry(name), fd(fd), oflags(oflags) {}

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
    if (!isOpen()) {
        throwInvalidOperation();
    }
    int ret = fsync(fd);
    if (ret == -1) {
        throw FsyncFailed();
    }
}

void anarion::FileChannel::symlink(const anarion::SString &linkPath) {
    int ret = ::symlink(path.getString().cstr(), linkPath.cstr());
    if (ret == -1) {
        throw LinkFailed();
    }
}

void anarion::FileChannel::hardlink(const anarion::SString &linkPath) {
    int ret = link(path.getString().cstr(), linkPath.cstr());
    if (ret < 0) {
        throw LinkFailed();
    }
}

void anarion::FileChannel::changePermission(int perm) {
    int ret = fchmod(fd, perm);
    if (ret < 0) {
        throw ChmodFailed();
    }
}

bool anarion::FileChannel::hasPermission(anarion::FileEntry::perm_t perm) {
    return perm & statInfo.st_mode;
}

void anarion::FileChannel::fetchAttributes() {
    int ret = ::fstat(fd, &statInfo);
    if (ret < 0) {
        throw StatFailed();
    }
}

const struct stat &anarion::FileChannel::getAttributes() const {
    return statInfo;
}

anarion::Time anarion::FileChannel::getLastAccessTime() {
    fetchAttributes();
    return Time(statInfo.st_atim);
}

anarion::Time anarion::FileChannel::getLastModifyTime() {
    fetchAttributes();
    return Time(statInfo.st_mtim);
}

anarion::Time anarion::FileChannel::getLastStatusChangeTime() {
    fetchAttributes();
    return Time(statInfo.st_ctim);
}
