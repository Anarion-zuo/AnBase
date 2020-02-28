#include <io/channel/Channel.h>
#include <sys/socket.h>
#include "io/buffer/Buffer.h"
#include "exceptions/io/FdWriteException.h"
#include "exceptions/io/FdReadException.h"
#include "exceptions/io/socket/SocketSendException.h"
#include <unistd.h>

using namespace anarion;

/*
    all xxxn functions serve the same purposes:
        1) read from system io to a buffer of certain length
        2) pack possible exception in cxx form

    therefore, they should follow the same coding pattern:
        1) iterative reading
        2) update buffer pointer at each term -- += read length
        3) terminate when
            a) having written the whole buffer
            b) system io drained
        4) return the number of effected bytes

    other attributes may also be added to the functions, as they must provide a most general interface
*/
namespace anarion {
    size_type writen(int fd, void *buf, size_type nbytes) {
        int len;
        size_type oldlen = nbytes;
        while (true) {
            len = ::write(fd, buf, nbytes);
            if (len < 0) { throw FdWriteException(); }
            nbytes -= len;
            buf = (char *) buf + len;
            if (len == 0 || nbytes == 0) { return oldlen - nbytes; }
        }
    }

    size_type readn(int fd, void *buf, size_type nbytes) {
        int len;
        size_type oldlen = nbytes;
        while (true) {
            len = ::read(fd, buf, nbytes);
            if (len < 0) { throw FdReadException(); }
            nbytes -= len;
            buf = (char *) buf + len;
            if (len == 0 || nbytes == 0) { return oldlen - nbytes; }
        }
    }

    size_type sendn(int cfd, void *buf, size_type nbytes, int flags) {
        int len;
        size_type oldn = nbytes;
        while (true) {
            len = ::send(cfd, buf, nbytes, flags);
            if (len < 0) { throw SocketSendException(); }
            buf = (char *) buf + len;
            nbytes -= len;
            if (len == 0 || nbytes == 0) { return oldn - nbytes; }
        }
    }

    size_type recvn(int cfd, void *buf, size_type nbytes, int flags) {
        int len;
        size_type oldn = nbytes;
        while (true) {
            len = ::recv(cfd, buf, nbytes, flags);
            if (len < 0) { throw SocketSendException(); }
            buf = (char *) buf + len;
            nbytes -= len;
            if (len == 0 || nbytes == 0) { return oldn - nbytes; }
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


/*
    the following functions transfer data from classic c array to a buffer
    they follow the same pattern:
        1) obtain array length
        2) call insert with automatically increment of capacity
        3) update member pointers
        4) check index out of range in reads
*/

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

/*
    the following functions that writes to the buffer 
    cannot directly call insert of Vector<char>
    they still require increment in capacity

    the policy is to increment the capacity of the buffer to 1.5 of its required amount

    the pointer pos must still be updated
*/

size_type Buffer::append_fd(int fd, size_type nbytes) {
    // special handling
    size_type ret = 0;
    if (nbytes > unwritten()) {
        size_type newsize = nbytes + size();
        newsize = newsize + (newsize >> 1);
        resize(newsize);
    }
    // space enough
    size_type len = readn(fd, cur, nbytes);
    cur += len;
    return len;
}

size_type Buffer::write_fd(int fd, size_type nbytes) {
    if (nbytes > unread()) { throw IndexOutOfRange(); }
    size_type len = writen(fd, pos, nbytes);
    pos += len;
    return len;
}

size_type Buffer::send_fd(int cfd, size_type nbytes, int flags) {
    if (nbytes > unread()) { throw IndexOutOfRange(); }
    size_type len = sendn(cfd, pos, nbytes, flags);
    pos += len;
    return len;
}

/**
 * the function receives from a network pipe to drain all data present in the pipe
 * required capacity of the buffer is not known in advance, 
 * therefore it must be checked and carefully handled at each term of iteration
 */

size_type Buffer::recv_fd(int cfd, int flags) {
    size_type ret = 0;   // keep record of recved bytes
    if (capacity() == 0) {
        resize(128);
    }
    while (true) {
        size_type nbytes = unwritten();
        size_type len = recvn(cfd, cur, nbytes, flags);
        cur += len;
        ret += len;
        if (len < nbytes) {
            // the pipe is drained
            return ret;
        }
        // pipe not drained, expand capacity
        resize(capacity() << 1);
    }
}

size_type Buffer::recv_fd(int cfd, size_type nbytes, int flags) {
    size_type ret = 0, newsize = nbytes + size();
    if (newsize > capacity()) {
        newsize = newsize + (newsize >> 1);
        resize(newsize);
    }
    size_type len = recvn(cfd, cur, nbytes, flags);
    cur += len;
    return len;
}

static bool str_has_c(char c, char *arr, size_type len) {
    for (size_type i = 0; i < len; ++i) {
        if (arr[i] == c) {
            return true;
        }
    }
    return false;
}

size_type Buffer::skip(char *cs, size_type len) {
    char *old = pos;
    while (str_has_c(*pos, cs, len)) {
        ++pos;
    }
    return pos - old;
}

size_type Buffer::skip(const char *cs) {
    return skip(const_cast<char*>(cs), ::strlen(cs));
}

Buffer Buffer::write_arr_to(char c) {
    size_type index = index_of(c);
    Buffer buffer(index);
    buffer.append_arr(*this, index);
    return move(buffer);
}

size_type Buffer::index_of(char c) const {
    char *p = pos;
    while (p < cur) {
        if (*p == c) {
            return p - pos;
        }
        ++p;
    }
    return p - pos;
}

void Buffer::append_arr(Buffer &buffer, size_type len) {
    if (len > buffer.unread()) {
        throw IndexOutOfRange();
    }
    size_type new_size = len + unwritten();
    if (len > unwritten()) {
        new_size = new_size + (new_size >> 1);
        resize(new_size);
    }
    append_arr(buffer.pos, len);
    buffer.pos += len;
}

void Buffer::write_arr(Buffer &buffer, size_type len) {
    buffer.append_arr(*this, len);
}

void Buffer::print() {
    for (size_type i = 0; i < size(); ++i) {
        printf("%c", begin[i]);
    }
    printf("\n");
}

