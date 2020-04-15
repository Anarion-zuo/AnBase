#include "container/SString.h"
#include <sys/file.h>
#include "exceptions/io/OpenFdException.h"
#include <sys/file.h>
#include <exceptions/InvalidOperation.h>
#include <io/channel/file/FileChannel.h>
#include <sys/stat.h>

//using namespace anarion;

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
    return writen(fd, p, nbytes);
}

anarion::size_type anarion::FileChannel::in(Buffer &buffer) {
    if (!valid()) { throw InvalidOperation(); }
    return buffer.write_fd(fd, buffer.unread());
}

anarion::size_type anarion::FileChannel::in(Buffer &buffer, size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    if (nbytes > buffer.unread()) { throw IndexOutOfRange(); }
    return buffer.write_fd(fd, nbytes);
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
    timespec mod = buf.st_mtim;
    if (time.tv_sec > mod.tv_sec) {
        return false;
    }
    if (time.tv_sec < mod.tv_sec) {
        return true;
    }
    return time.tv_nsec < mod.tv_nsec;
}

void anarion::FileChannel::open() {
    return;
}

void anarion::FileChannel::release() {
    close();
}

anarion::size_type anarion::FileChannel::in(anarion::FixedBuffer &buffer) {
    if (!valid()) { throw InvalidOperation(); }
    return buffer.write_fd(fd, buffer.unread());
}

anarion::size_type anarion::FileChannel::in(anarion::FixedBuffer &buffer, anarion::size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    if (nbytes > buffer.unread()) { throw IndexOutOfRange(); }
    return buffer.write_fd(fd, nbytes);
}

anarion::FixedBuffer anarion::FileChannel::outBuffer(anarion::size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    FixedBuffer buffer(nbytes);
    buffer.append_fd(fd, nbytes);
    return move(buffer);
}

anarion::FixedBuffer anarion::FileChannel::outBuffer() {
    if (!o_valid) { throw InvalidOperation(); }
    off_t cur = ::lseek(fd, 0, SEEK_CUR);
    off_t size = ::lseek(fd, 0, SEEK_END);
    ::lseek(fd, cur, SEEK_SET);
    off_t unread = size - cur;
    return outBuffer(unread);
}
