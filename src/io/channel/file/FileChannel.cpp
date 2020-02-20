#include "FileChannel.h"
#include "container/SString.h"
#include <sys/file.h>
#include "exceptions/io/OpenFdException.h"
#include <sys/file.h>

using namespace anarion;

FileChannel FileChannel::open(const SString &dir) {
    int fd = ::open(dir.cstr(), O_RDWR);
    if (fd < 0) { throw OpenFdException(); }
    return FileChannel(fd);
}

void FileChannel::close() {
    if (valid()) {
        is_valid = false;
        ::close(fd);
    }
}

void FileChannel::rewind() {
    ::lseek(fd, 0, SEEK_SET);
}

void FileChannel::set_append() {
    ::lseek(fd, 0, SEEK_END);
}

void FileChannel::move_forth(size_type nbytes) {
    ::lseek(fd, nbytes, SEEK_CUR);
}

void FileChannel::move_back(size_type nbytes) {
    ::lseek(fd, -nbytes, SEEK_CUR);
}

size_type FileChannel::size() const {
    return ::lseek(fd, 0, SEEK_CUR);
}

size_type FileChannel::in(char *p, size_type nbytes) {
    return writen(fd, p, nbytes);
}

size_type FileChannel::in(Buffer &buffer) {
    return buffer.write_fd(fd, buffer.unread());
}

size_type FileChannel::in(Buffer &buffer, size_type nbytes) {
    if (nbytes > buffer.unread()) { throw IndexOutOfRange(); }
    return buffer.write_fd(fd, nbytes);
}

size_type FileChannel::out(char *p, size_type nbytes) {
    return readn(fd, p, nbytes);
}

Buffer FileChannel::out(size_type nbytes) {
    Buffer buffer(nbytes);
    buffer.append_fd(fd, nbytes);
    return move(buffer);
}
