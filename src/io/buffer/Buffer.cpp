//
// Created by anarion on 4/14/20.
//

#include <exceptions/container/IndexOutOfRange.h>
#include <io/buffer/TTBuffer.h>
#include <sys/socket.h>
#include "exceptions/io/FdWriteException.h"
#include "exceptions/io/FdReadException.h"
#include "exceptions/io/socket/SocketSendException.h"
#include <unistd.h>
#include <cstdio>

namespace anarion {
    anarion::size_type writen(int fd, void *buf, anarion::size_type nbytes) {
        int len;
        anarion::size_type oldlen = nbytes;
        while (true) {
            len = ::write(fd, buf, nbytes);
            if (len < 0) { throw FdWriteException(); }
            nbytes -= len;
            buf = (char *) buf + len;
            if (len == 0 || nbytes == 0) { return oldlen - nbytes; }
        }
    }

    anarion::size_type readn(int fd, void *buf, anarion::size_type nbytes) {
        int len;
        anarion::size_type oldlen = nbytes;
        while (true) {
            len = ::read(fd, buf, nbytes);
            if (len < 0) { throw FdReadException(); }
            nbytes -= len;
            buf = (char *) buf + len;
            if (len == 0 || nbytes == 0) { return oldlen - nbytes; }
        }
    }

    anarion::size_type sendn(int cfd, void *buf, anarion::size_type nbytes, int flags) {
        int len;
        anarion::size_type oldn = nbytes;
        while (true) {
            len = ::send(cfd, buf, nbytes, flags);
            if (len < 0) { throw SocketSendException(); }
            buf = (char *) buf + len;
            nbytes -= len;
            if (len == 0 || nbytes == 0) { return oldn - nbytes; }
        }
    }

    anarion::size_type recvn(int cfd, void *buf, anarion::size_type nbytes, int flags) {
        int len;
        anarion::size_type oldn = nbytes;
        while (true) {
            len = ::recv(cfd, buf, nbytes, flags);
            if (len < 0) { throw SocketSendException(); }
            buf = (char *) buf + len;
            nbytes -= len;
            if (len == 0 || nbytes == 0) { return oldn - nbytes; }
        }
    }
}

anarion::HashMap<anarion::size_type, anarion::FixedLengthAllocator*> anarion::TTBuffer::allocatorMap;
anarion::Mutex anarion::TTBuffer::allocatorMapLock;

void anarion::TTBuffer::newFrame() {
    void *p = allocator->allocate();
    frames.push_back(p);
}

void anarion::TTBuffer::newFrames(anarion::size_type n) {
    for (size_type i = 0; i < n; ++i) {
        newFrame();
    }
}

anarion::FixedLengthAllocator *anarion::TTBuffer::getAllocator(anarion::size_type frameLength) {
    allocatorMapLock.lock();
    auto it = allocatorMap.find(frameLength);
    if (it == allocatorMap.end_iterator()) {
        allocatorMap.insert({frameLength, new FixedLengthAllocator(frameLength)});
        it = allocatorMap.find(frameLength);
    }
    FixedLengthAllocator *ret = it->get_val();
    allocatorMapLock.unlock();
    return ret;
}

anarion::TTBuffer::TTBuffer(const unsigned long frameLength) :
    frameLength(frameLength), allocator(getAllocator(frameLength))
{
    newFrame();
    rewind();
    writeIterator = readIterator;
}

void anarion::TTBuffer::clear() {
    for (auto it = frames.begin_iterator(); it != frames.end_iterator(); ++it) {
        allocator->deallocate(*it);
    }
}

anarion::TTBuffer::TTBuffer(const anarion::TTBuffer &rhs) :
    frameLength(rhs.frameLength), allocator(rhs.allocator)
{
    // copy each frame
    for (auto it = rhs.frames.begin_iterator(); it != rhs.frames.end_iterator(); ++it) {
        void *p = allocator->allocate();
        memcpy(p, *it, frameLength);
        frames.push_back(p);
    }
    // write index
    writeIterator.listIndex = rhs.writeIterator.listIndex;
    auto writeIt = frames.begin_iterator();
    for (size_t i = 0; i < writeIterator.listIndex; ++i) {
        ++writeIt;
    }
    writeIterator.listIt = writeIt;
    // read index
    rewind();
}

void anarion::TTBuffer::rewind() {
    readIterator.listIndex = 0;
    readIterator.listIt = frames.begin_iterator();
}

void anarion::TTBuffer::refresh() {
    rewind();
    writeIterator = readIterator;
}

anarion::TTBuffer::TTBuffer(anarion::TTBuffer &&rhs) noexcept :
    frameLength(rhs.frameLength), allocator(rhs.allocator), frames(move(rhs.frames)), readIterator(move(rhs.readIterator)), writeIterator(move(rhs.writeIterator)) {

}

void anarion::TTBuffer::allocateNew(anarion::size_type more_size) {
    size_type n = more_size / frameLength, m = more_size % frameLength;
    if (m != 0) {
        ++n;
    }
    newFrames(n);
}

void anarion::TTBuffer::setWriteIndex(anarion::size_type index) {
    if (index > size()) {
        throw IndexOutOfRange();
    }
    size_type n = index / frames.size(), m = index % frames.size();
    ++n;
    auto it = frames.begin_iterator();
    for (size_type i = 0; i < n; ++i) {
        ++it;
    }
    writeIterator.listIt = it;
    writeIterator.offset = m;
    writeIterator.listIndex = n;
}

void anarion::TTBuffer::append_arr(char *p, anarion::size_type len) {
    if (p == nullptr || len == 0) { return; }
    // expand memory occupation
    if (len > unwritten()) {
        allocateNew(len - unwritten());
    }
    // inject data
    size_type n = 0;
    while (true) {
        n = iteratorUnTouched(writeIterator);
        if (len < n) {
            n = len;
        }
        memcpy(writeIterator.offsettedPtr(), p, n);
        len -= n;
        p += n;
        if (len == 0) {
            break;
        }
        iteratorNext(writeIterator);
    }
    writeIterator.increaseOffset(n);
}

void anarion::TTBuffer::append_arr(const char *str) {
    append_arr(const_cast<char*>(str), strlen(str));
}

void anarion::TTBuffer::write_arr(char *p, anarion::size_type len) {
    if (p == nullptr || len == 0) {
        return;
    }
    if (len > unread()) {
        throw IndexOutOfRange();
    }
    size_type n = 0;
    while (true) {
        n = iteratorUnTouched(readIterator);
        if (len < n) {
            n = len;
        }
        memcpy(p, readIterator.offsettedPtr(), n);
        len -= n;
        p += n;
        if (len == 0) {
            break;
        }
        iteratorNext(readIterator);
    }
    readIterator.increaseOffset(n);
}

void anarion::TTBuffer::append_arr(anarion::TTBuffer &buffer, anarion::size_type len) {

}

void anarion::TTBuffer::write_arr(anarion::TTBuffer &buffer, anarion::size_type len) {

}

anarion::size_type anarion::TTBuffer::append_fd(int fd, anarion::size_type nbytes) {
    if (nbytes == 0) { return 0; }
    // expand memory occupation
    if (nbytes > unwritten()) {
        allocateNew(nbytes - unwritten());
    }
    // inject data
    size_type n = 0, len = 0;
    while (true) {
        n = iteratorUnTouched(writeIterator);
        if (nbytes < n) {
            n = nbytes;
        }
        size_type ret = anarion::readn(fd, writeIterator.offsettedPtr(), n);
        len += ret;
        nbytes -= ret;
        if (ret < n) {
            n = ret;
            break;
        }
        if (nbytes == 0) {
            break;
        }
        iteratorNext(writeIterator);
    }
    writeIterator.increaseOffset(n);
    return len;
}

anarion::size_type anarion::TTBuffer::append_fd(int fd) {
    // inject data
    size_type n = 0, len = 0;
    while (true) {
        n = iteratorUnTouched(writeIterator);
        size_type ret = readn(fd, writeIterator.offsettedPtr(), n);
        len += ret;
        if (ret < n) {
            n = ret;
            break;
        }
        // check if more space needed
        iterator temp = writeIterator;
        iteratorNext(writeIterator);
        if (iteratorIsEnd(writeIterator)) {
            newFrame();
            writeIterator = temp;
            iteratorNext(writeIterator);
        }
    }
    writeIterator.increaseOffset(n);
    return len;
}

anarion::size_type anarion::TTBuffer::write_fd(int fd, anarion::size_type nbytes) {
    if (nbytes == 0) { return 0; }
    if (nbytes > unread()) {
        throw IndexOutOfRange();
    }
    size_type n = 0, len = 0;
    while (true) {
        n = iteratorUnTouched(readIterator);
        if (nbytes < n) {
            n = nbytes;
        }
        size_type ret = writen(fd, readIterator.offsettedPtr(), n);
        nbytes -= ret;
        len += ret;
        if (ret < n) {
            n = ret;
            break;
        }
        if (nbytes == 0) {
            break;
        }
        iteratorNext(readIterator);
    }
    readIterator.increaseOffset(n);
    return len;
}

anarion::size_type anarion::TTBuffer::send_fd(int cfd, anarion::size_type nbytes, int flags) {
    if (nbytes == 0) { return 0; }
    if (nbytes > unread()) {
        throw IndexOutOfRange();
    }
    size_type n = 0, len = 0;
    while (true) {
        n = iteratorUnTouched(readIterator);
        if (nbytes < n) {
            n = nbytes;
        }
        size_type ret = sendn(cfd, readIterator.offsettedPtr(), n, flags);
        nbytes -= ret;
        len += ret;
        if (ret < n) {
            n = ret;
            break;
        }
        if (nbytes == 0) {
            break;
        }
        iteratorNext(readIterator);
    }
    readIterator.increaseOffset(n);
    return len;
}

anarion::size_type anarion::TTBuffer::recv_fd(int cfd, int flags) {
    // inject data
    size_type n = 0, len = 0;
    while (true) {
        n = iteratorUnTouched(writeIterator);
        size_type ret = recv(cfd, writeIterator.offsettedPtr(), n, flags);
        len += ret;
        if (ret < n) {
            n = ret;
            break;
        }
        // check if more space needed
        iterator temp = writeIterator;
        iteratorNext(writeIterator);
        if (iteratorIsEnd(writeIterator)) {
            newFrame();
            writeIterator = temp;
            iteratorNext(writeIterator);
        }
    }
    writeIterator.increaseOffset(n);
    return len;
}

anarion::size_type anarion::TTBuffer::recv_fd(int cfd, anarion::size_type nbytes, int flags) {
    if (nbytes == 0) { return 0; }
    // expand memory occupation
    if (nbytes > unwritten()) {
        allocateNew(nbytes - unwritten());
    }
    // inject data
    size_type n = 0, len = 0;
    while (true) {
        n = iteratorUnTouched(writeIterator);
        if (nbytes < n) {
            n = nbytes;
        }
        size_type ret = recvn(cfd, writeIterator.offsettedPtr(), n, flags);
        len += ret;
        nbytes -= ret;
        if (ret < n) {
            n = ret;
            break;
        }
        if (nbytes == 0) {
            break;
        }
        iteratorNext(writeIterator);
    }
    writeIterator.increaseOffset(n);
    return len;
}

void anarion::TTBuffer::print() {
    char *str = static_cast<char *>(operator new(size() + 1));
    write_arr(str, size());
    str[size()] = 0;
    printf("%s\n", str);
    operator delete (str, size() + 1);
}




