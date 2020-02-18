#include "Buffer.h"
#include <unistd.h>
#include "exceptions/io/FdWriteException.h"
#include "exceptions/io/FdReadException.h"

using namespace anarion;

size_type writen(int fd, void *buf, size_type nbytes) {
    int len;
    size_type oldlen = nbytes;
    while (true) {
        len = ::write(fd, buf, nbytes);
        if (len < 0) { throw FdWriteException(); }
        if (len == 0) { return oldlen - nbytes; }
        if (len < nbytes) {
            nbytes -= len;
        }
    }
}

size_type readn(int fd, void *buf, size_type nbytes) {
    int len;
    size_type oldlen = nbytes;
    while (true) {
        len = ::read(fd, buf, nbytes);
        if (len < 0) { throw FdReadException(); }
        if (len == 0) { return oldlen - nbytes; }
        if (len < nbytes) {
            nbytes -= len;
        }
    }
}

Buffer &Buffer::operator=(const Buffer &rhs) {
    if (*this == rhs) { return *this; }
    parent::operator=(rhs);
    pos = rhs.pos - rhs.begin + begin;
    return *this;
}

Buffer &Buffer::operator=(Buffer &&rhs) noexcept {
    parent::operator=(forward<Buffer>(rhs));
    pos = rhs.pos;
    rhs.pos = nullptr;
    return *this;
}

void Buffer::append_arr(char *p, size_type len) {
    insert(cur, p, len);
}

void Buffer::append_arr(const char *str) {
    size_type len = ::strlen(str);
    append_arr(const_cast<char*>(str), len);
}

void Buffer::write_arr(char *p, size_type len) {
    if (len > unread()) { throw IndexOutOfRange(); }
    ::memcpy(p, pos, len);
    pos += len;
}

size_type Buffer::append_fd(int fd, size_type nbytes) {
    // special handling
    while (true) {
        size_type temp_len = nbytes;
        if (nbytes > unwritten()) {
            temp_len = unwritten();
        }
        int len = readn(fd, cur, temp_len);
        if ()
    }
    if (nbytes > unwritten()) { throw IndexOutOfRange(); }
    size_type len = readn(fd, cur, nbytes);
    cur += len;
    return len;
}

size_type Buffer::write_fd(int fd, size_type nbytes) {
    size_type len = writen(fd, pos, nbytes);
    pos += len;
    return len;
}


