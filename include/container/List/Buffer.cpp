//
// Created by 左钰 on 2020/1/24.
//

#include <unistd.h>
#include <sys/socket.h>
#include "Buffer.h"
#include "../../exceptions/io/FdReadException.h"

using namespace anarion;

Buffer &Buffer::operator=(const Buffer &rhs) {
    if (this == &rhs) {
        return *this;
    }
    Vector<char>::operator=(rhs);
    pos = rhs.pos;
    return *this;
}

Buffer &Buffer::operator=(Buffer &&rhs) noexcept {
    Vector<char>::operator=(std::forward<Buffer>(rhs));
    pos = rhs.pos;
    return *this;
}

size_type Buffer::unReadSize() const {
    return cur - pos;
}

size_type Buffer::unWrittenSize() const {
    return end - cur;
}

void Buffer::put(char *arr, size_type *len) {
    insert(cur, arr, *len);
}

void Buffer::put(int fd) {
    size_type len = unWrittenSize();
    int num = len;
    while (true) {
        num = ::read(fd, cur, len);
        if (num == -1) {
            throw FdReadException();
        }
        cur += num;
        if (num <= len) {
            break;
        }
        expand_push();
        len = unWrittenSize();
    }
}

bool Buffer::recv(int cfd) {
    size_type len = unWrittenSize();
    int num = len;
    while (true) {
        num = ::recv(cfd, cur, len, 0);
        if (num == -1) {
            throw FdReadException();
        }
        if (num == 0) {
            return false;
        }
        cur += num;
        if (num <= len) {
            break;
        }
        expand_push();
        len = unWrittenSize();
    }
    return true;
}

void Buffer::resetRead() {
    pos = begin;
}

char Buffer::get() {
    if (cur <= pos) {
        throw IndexOutOfRange();
    }
    char c = *pos;
    ++pos;
    return c;
}

void Buffer::get(char *arr, size_type num) {
    if (num > unReadSize()) {
        throw IndexOutOfRange();
    }
    memcpy(arr, pos, num);
    pos += num;
}

size_type Buffer::indexOf(char c) const {
    size_type index = 0;
    while (index < size()) {
        if (begin[index] == c) {
            return index;
        }
    }
    return index;
}

size_type Buffer::indexOfSinceRead(char c) const {
    size_type index = pos - begin, rec = index;
    while (index < size()) {
        if (begin[index] == c) {
            return index - rec;
        }
        ++index;
    }
    return index - rec;
}

size_type Buffer::getUntil(char c, char *arr) {
    size_type index = indexOfSinceRead(c);
    get(arr, index);
    return index;
}

Buffer *Buffer::getUntil(char c) {
    size_type index = indexOfSinceRead(c);
//    Buffer *buffer = newObject<Buffer>(index);
    Buffer *buffer = new Buffer(index);
    memcpy(buffer->begin, pos, index);
    buffer->cur += index;
    pos += index;
    return buffer;
}

void Buffer::getRest(char *arr) {
    size_type rest = unReadSize();
    get(arr, rest);
}

void Buffer::skip(char *cs, size_type len) {
    while (pos < cur) {
        bool flag = false;
        for (size_type i = 0; i < len; ++i) {
            if (*pos == cs[i]) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            return;
        }
        ++pos;
    }
}

Buffer::Buffer(char *buf, size_type size, size_type cap) {
    begin = buf;
    cur = buf + size;
    end = buf + cap;
    pos = begin;
}

void Buffer::resetWrite() {
    cur = begin;
    pos = begin;
}

void Buffer::writeFd(int fd, size_type len) {

    pos += len;
}
