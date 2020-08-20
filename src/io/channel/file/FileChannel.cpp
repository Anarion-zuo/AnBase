#include "container/SString.h"
#include <exceptions/InvalidOperation.h>
#include <io/channel/file/FileChannel.h>
#include <sys/stat.h>

anarion::FileChannel anarion::FileChannel::open(const SString &dir) {
    char *cdir = dir.copy_cstr();
    int fd = ::open(cdir, O_RDWR | O_CREAT, 0666);
    dir.release_copied(cdir);
    if (fd < 0) { throw OpenFdException(); }
    return FileChannel(SString(dir), fd);
}

void anarion::FileChannel::close() {
    if (valid()) {
        set_invalid();
        ::close(fd);
    }
}

void anarion::FileChannel::rewind() {
    if (!valid()) { throw InvalidOperation(); }
    ::lseek(fd, 0, SEEK_SET);
}

void anarion::FileChannel::set_append() {
    if (!valid()) { throw InvalidOperation(); }
    ::lseek(fd, 0, SEEK_END);
}

void anarion::FileChannel::move_forth(size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    ::lseek(fd, nbytes, SEEK_CUR);
}


void anarion::FileChannel::move_back(size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    ::lseek(fd, -nbytes, SEEK_CUR);
}

anarion::size_type anarion::FileChannel::size() const {
    if (!valid()) { throw InvalidOperation(); }
    off_t off = ::lseek(fd, 0, SEEK_CUR);
    size_type ret = ::lseek(fd, 0, SEEK_END);
    ::lseek(fd, off, SEEK_SET);
    return ret;
}

anarion::size_type anarion::FileChannel::in(char *p, size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    size_type ret = writen(fd, p, nbytes);
    return ret;
}

anarion::size_type anarion::FileChannel::in(Buffer &buffer) {
    if (!valid()) { throw InvalidOperation(); }
    size_type ret = buffer.write_fd(fd, buffer.unread());
    return ret;
}

anarion::size_type anarion::FileChannel::in(Buffer &buffer, size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    if (nbytes > buffer.unread()) { throw IndexOutOfRange(); }
    size_type ret = buffer.write_fd(fd, nbytes);
    return ret;
}

anarion::size_type anarion::FileChannel::out(char *p, size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    return readn(fd, p, nbytes);
}

anarion::Buffer anarion::FileChannel::out(size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    Buffer buffer(nbytes);
    buffer.append_fd(fd, nbytes);
    return move(buffer);
}

anarion::Buffer anarion::FileChannel::out() {
    if (!o_valid) { throw InvalidOperation(); }
    off_t cur = ::lseek(fd, 0, SEEK_CUR);
    off_t size = ::lseek(fd, 0, SEEK_END);
    ::lseek(fd, cur, SEEK_SET);
    off_t unread = size - cur;
    return out(unread);
}

bool anarion::FileChannel::modifiedLaterThan(const timespec &time) {
    struct ::stat buf;
    ::fstat(fd, &buf);
#ifdef __APPLE__
    timespec mod = buf.st_mtimespec;
#elif __linux__
    timespec mod = buf.st_mtim;
#endif
    if (time.tv_sec > mod.tv_sec) {
        return false;
    }
    if (time.tv_sec < mod.tv_sec) {
        return true;
    }
    if (time.tv_nsec < mod.tv_nsec) {
        return true;
    }
    return false;
}

void anarion::FileChannel::open() {
    return;
}

void anarion::FileChannel::release() {
    close();
}

void anarion::FileChannel::remove() {
    close();
    char *p = absolutePath.copy_cstr();
    ::remove(p);
    absolutePath.release_copied(p);

    if (parent) {
        parent->removeChildFromMembers(this);
    }
}

bool anarion::FileChannel::isFile() const {
    return true;
}

void anarion::FileChannel::initOpen(const char *cdir, int oflag, int perm) {
    fd = ::open(cdir, oflag, perm);
    if (fd < 0) {
        set_invalid();
        throw OpenFdException();
    }
    ::fstat(fd, &statInfo);
}

anarion::FileChannel::FileChannel(const anarion::SString &dir)
    : InChannel(true), OutChannel(true), RandomChannel(true), FileEntry(SString(dir)) {
    initOpen(dir.getArr(), O_RDWR | O_CREAT, 0666);
}

anarion::FileChannel::FileChannel(const anarion::SString &dir, int oflag)
    : InChannel(true), OutChannel(true), RandomChannel(true), FileEntry(SString(dir)) {
    initOpen(dir.getArr(), oflag, 0666);
}

anarion::FileChannel::FileChannel(const anarion::SString &dir, int oflag, int perm)
    : InChannel(true), OutChannel(true), RandomChannel(true), FileEntry(SString(dir)) {
    initOpen(dir.getArr(), oflag, perm);
}

//anarion::size_type anarion::FileChannel::in(anarion::Buffer &buffer) {
//    if (!valid()) { throw InvalidOperation(); }
//    return buffer.write_fd(fd, buffer.unread());
//}
//
//anarion::size_type anarion::FileChannel::in(anarion::Buffer &buffer, anarion::size_type nbytes) {
//    if (!valid()) { throw InvalidOperation(); }
//    if (nbytes > buffer.unread()) { throw IndexOutOfRange(); }
//    return buffer.write_fd(fd, nbytes);
//}
//
//anarion::Buffer anarion::FileChannel::outBuffer(anarion::size_type nbytes) {
//    if (!valid()) { throw InvalidOperation(); }
//    Buffer buffer(nbytes);
//    buffer.append_fd(fd, nbytes);
//    return move(buffer);
//}
//
//anarion::Buffer anarion::FileChannel::outBuffer() {
//    if (!o_valid) { throw InvalidOperation(); }
//    off_t cur = ::lseek(fd, 0, SEEK_CUR);
//    off_t size = ::lseek(fd, 0, SEEK_END);
//    ::lseek(fd, cur, SEEK_SET);
//    off_t unread = size - cur;
//    return outBuffer(unread);
//}
