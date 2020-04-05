#ifndef BUFFER_H
#define BUFFER_H

#include "../../container/List/Vector.hpp"
#include "../base/sys_utility.h"

namespace anarion {

class Buffer : public Vector<char> {
    friend class SString;
protected:

    typedef Vector<char> parent;

    char *pos = nullptr;

public:
    
    Buffer() = default;
    explicit Buffer(size_type nbytes) : Vector<char>(nbytes), pos(Vector<char>::begin) {}
    Buffer(const Buffer &rhs) : Vector<char>(rhs), pos(rhs.pos - rhs.begin + begin) {}
    Buffer(Buffer &&rhs) noexcept : Vector<char>(forward<Buffer>(rhs)), pos(rhs.pos) { rhs.pos = nullptr; }
    ~Buffer() = default;

    Buffer &operator=(const Buffer &rhs);
    Buffer &operator=(Buffer &&rhs) noexcept ;

    void resize(size_type newsize);  // must update pos member, or corrupt heap

    // lengths
    size_type unread() const { return cur - pos; }
    size_type unwritten() const { return end - cur; }
    constexpr void rewind() { pos = begin; }

    // arr
    void append_arr(char *p, size_type len);
    void append_arr(const char *str);
    void write_arr(char *p, size_type len);
    void append_arr(Buffer &buffer, size_type len);
    void write_arr(Buffer &buffer, size_type len);

    // file descriptor
    size_type append_fd(int fd, size_type nbytes);
    size_type write_fd(int fd, size_type nbytes);
    size_type send_fd(int cfd, size_type nbytes, int flags);
    size_type recv_fd(int cfd, int flags);
    size_type recv_fd(int cfd, size_type nbytes, int flags);

    // char
    size_type skip(char *cs, size_type len);
    size_type skip(const char *cs);
    constexpr void skip() { ++pos; }
    size_type index_of(char c) const ;  // from pos on
    Buffer write_arr_to(char c);

    void print();  // debug

};

size_type writen(int fd, void *buf, size_type nbytes);
size_type readn(int fd, void *buf, size_type nbytes);
size_type sendn(int cfd, void *buf, size_type nbytes, int flags);
size_type recvn(int cfd, void *buf, size_type nbytes, int flags);

}
#endif // BUFFER_H