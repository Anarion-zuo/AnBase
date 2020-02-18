#ifndef BUFFER_H
#define BUFFER_H

#include "../../container/List/Vector.hpp"

namespace anarion {
class Buffer : public Vector<char> {
protected:

    typedef Vector<char> parent;

    char *pos = nullptr;

public:
    
    Buffer() = default;
    Buffer(size_type nbytes) : Vector<char>(nbytes), pos(Vector<char>::begin) {}
    Buffer(const Buffer &rhs) : Vector<char>(rhs), pos(rhs.pos - rhs.begin + begin) {}
    Buffer(Buffer &&rhs) noexcept : Vector<char>(forward<Buffer>(rhs)), pos(rhs.pos) { rhs.pos = nullptr; }
    ~Buffer() = default;

    Buffer &operator=(const Buffer &rhs);
    Buffer &operator=(Buffer &&rhs) noexcept ;

    // lengths
    size_type unread() const { return cur - pos; }
    size_type unwritten() const { return end - pos; }
    size_type readed() const { return pos - begin; }

    // arr
    void append_arr(char *p, size_type len);
    void append_arr(const char *str);
    void write_arr(char *p, size_type len);

    // file descriptor
    size_type append_fd(int fd, size_type nbytes);
    size_type write_fd(int fd, size_type nbytes);


};
size_type  writen(int fd, void *buf, size_type nbytes);
size_type  readn(int fd, void *buf, size_type nbytes);
// void sendn(int );
}
#endif // BUFFER_H