//
// Created by anarion on 4/14/20.
//

#include <exceptions/container/IndexOutOfRange.h>
#include "io/buffer/FixedBuffer.h"
#include <io/buffer/Buffer.h>
#include <sys/socket.h>

anarion::HashMap<anarion::size_type, anarion::FixedLengthAllocator*> anarion::FixedBuffer::allocatorMap;

void anarion::FixedBuffer::newFrame() {
    void *p = allocator->allocate();
    frames.push_back(p);
}

void anarion::FixedBuffer::newFrames(anarion::size_type n) {
    for (size_type i = 0; i < n; ++i) {
        newFrame();
    }
}

anarion::FixedLengthAllocator *anarion::FixedBuffer::getAllocator(anarion::size_type frameLength) {
    auto it = allocatorMap.find(frameLength);
    if (it == allocatorMap.end_iterator()) {
        allocatorMap.insert({frameLength, new FixedLengthAllocator(frameLength)});
        it = allocatorMap.find(frameLength);
    }
    return it->get_val();
}

anarion::FixedBuffer::FixedBuffer(const unsigned long frameLength) :
    frameLength(frameLength), allocator(getAllocator(frameLength))
{
    newFrame();
    rewind();
    writeIterator = readIterator;
}

void anarion::FixedBuffer::clear() {
    for (auto it = frames.begin_iterator(); it != frames.end_iterator(); ++it) {
        allocator->deallocate(*it);
    }
}

anarion::FixedBuffer::FixedBuffer(const anarion::FixedBuffer &rhs) :
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

void anarion::FixedBuffer::rewind() {
    readIterator.listIndex = 0;
    readIterator.listIt = frames.begin_iterator();
}

anarion::FixedBuffer::FixedBuffer(anarion::FixedBuffer &&rhs) noexcept :
    frameLength(rhs.frameLength), allocator(rhs.allocator), frames(move(rhs.frames)), readIterator(move(rhs.readIterator)), writeIterator(move(rhs.writeIterator)) {

}

void anarion::FixedBuffer::allocateNew(anarion::size_type more_size) {
    size_type n = more_size / frameLength, m = more_size % frameLength;
    if (m != 0) {
        ++n;
    }
    newFrames(n);
}

void anarion::FixedBuffer::resize(size_type new_size) {
    size_type more = new_size - capacity();
    size_type n_frames = more / frameLength, m_frames = more % frameLength;
    if (m_frames) {
        ++n_frames;
    }
    if (new_size > capacity()) {
        newFrames(n_frames);
        return;
    }
    size_type del_num = frames.size() - n_frames;
    for (size_type i = 0; i < del_num; ++i) {
        allocator->deallocate(frames.pop_back());
    }
    if (n_frames < frames.size()) {
        // remaining frames are less than written or read size

//        if ()
    }
}

void anarion::FixedBuffer::append_arr(char *p, anarion::size_type len) {
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

void anarion::FixedBuffer::append_arr(const char *str) {
    append_arr(const_cast<char*>(str), strlen(str));
}

void anarion::FixedBuffer::write_arr(char *p, anarion::size_type len) {
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

void anarion::FixedBuffer::append_arr(anarion::FixedBuffer &buffer, anarion::size_type len) {

}

void anarion::FixedBuffer::write_arr(anarion::FixedBuffer &buffer, anarion::size_type len) {

}

anarion::size_type anarion::FixedBuffer::append_fd(int fd, anarion::size_type nbytes) {
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
        size_type ret = readn(fd, writeIterator.offsettedPtr(), n);
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

anarion::size_type anarion::FixedBuffer::append_fd(int fd) {
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

anarion::size_type anarion::FixedBuffer::write_fd(int fd, anarion::size_type nbytes) {
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

anarion::size_type anarion::FixedBuffer::send_fd(int cfd, anarion::size_type nbytes, int flags) {
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

anarion::size_type anarion::FixedBuffer::recv_fd(int cfd, int flags) {
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

anarion::size_type anarion::FixedBuffer::recv_fd(int cfd, anarion::size_type nbytes, int flags) {
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

void anarion::FixedBuffer::print() {
    char *str = static_cast<char *>(operator new(size() + 1));
    write_arr(str, size());
    str[size()] = 0;
    printf("%s\n", str);
    operator delete (str, size() + 1);
}



