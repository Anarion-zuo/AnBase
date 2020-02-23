#include "FileChannel.h"
#include "container/SString.h"
#include <sys/file.h>
#include "exceptions/io/OpenFdException.h"
#include <sys/file.h>
#include <exceptions/InvalidOperation.h>

using namespace anarion;

FileChannel FileChannel::open(const SString &dir) {
    char *cdir = dir.copy_cstr();
    int fd = ::open(cdir, O_RDWR | O_CREAT, 0666);
    dir.release_copied(cdir);
    if (fd < 0) { throw OpenFdException(); }
    return FileChannel(fd);
}

void FileChannel::close() {
    if (valid()) {
        set_invalid();
        ::close(fd);
    }
}

void FileChannel::rewind() {
    if (!valid()) { throw InvalidOperation(); }
    ::lseek(fd, 0, SEEK_SET);
}

void FileChannel::set_append() {
    if (!valid()) { throw InvalidOperation(); }
    ::lseek(fd, 0, SEEK_END);
}

void FileChannel::move_forth(size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    ::lseek(fd, nbytes, SEEK_CUR);
}

void FileChannel::move_back(size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    ::lseek(fd, -nbytes, SEEK_CUR);
}

size_type FileChannel::size() const {
    if (!valid()) { throw InvalidOperation(); }
    off_t off = ::lseek(fd, 0, SEEK_CUR);
    size_type ret = ::lseek(fd, 0, SEEK_END);
    ::lseek(fd, off, SEEK_SET);
    return ret;
}

size_type FileChannel::in(char *p, size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    return writen(fd, p, nbytes);
}

size_type FileChannel::in(Buffer &buffer) {
    if (!valid()) { throw InvalidOperation(); }
    return buffer.write_fd(fd, buffer.unread());
}

size_type FileChannel::in(Buffer &buffer, size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    if (nbytes > buffer.unread()) { throw IndexOutOfRange(); }
    return buffer.write_fd(fd, nbytes);
}

size_type FileChannel::out(char *p, size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    return readn(fd, p, nbytes);
}

Buffer FileChannel::out(size_type nbytes) {
    if (!valid()) { throw InvalidOperation(); }
    Buffer buffer(nbytes);
    buffer.append_fd(fd, nbytes);
    return move(buffer);
}

Buffer FileChannel::out() {
    if (!o_valid) { InvalidOperation(); }
    off_t size = ::lseek(fd, 0, SEEK_END);
    off_t unread = size - ::lseek(fd, size, SEEK_SET);
    return out(unread);
}
