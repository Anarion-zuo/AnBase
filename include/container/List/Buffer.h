//
// Created by 左钰 on 2020/1/24.
//

#ifndef MYCPPLIB_BUFFER_H
#define MYCPPLIB_BUFFER_H

#include "Vector.hpp"

namespace anarion {
class Buffer : public Vector<char> {
    friend class CompressPipe;
    friend class FileChannel;
protected:
    char *pos = nullptr;

public:
    Buffer() : Vector<char>(1024), pos(Vector<char>::begin) {}
    Buffer(char *buf, size_type size, size_type cap);
    explicit Buffer(size_type num) : Vector<char>(num), pos(Vector<char>::begin) {}
    Buffer(const Buffer &rhs) : Vector<char>(rhs), pos(rhs.pos) {}
    Buffer(Buffer &&rhs) noexcept : Vector<char>(std::forward<Buffer>(rhs)), pos(rhs.pos) {}

    Buffer &operator=(const Buffer &rhs);
    Buffer &operator=(Buffer &&rhs) noexcept ;

    size_type unWrittenSize() const ;
    size_type unReadSize() const ;
    void put(char *arr, size_type *len);
    void put(int fd);
    bool recv(int cfd);
    void resetRead();
    void resetWrite();
    char get();
    void get(char *arr, size_type num);
    void writeFd(int fd, size_type len);
    size_type indexOf(char c) const ;
    size_type indexOfSinceRead(char c) const ;
    size_type getUntil(char c, char *arr);
    Buffer *getUntil(char c);
    void getRest(char *arr);
    void skip(char *cs, size_type len);
};
}

#endif //MYCPPLIB_BUFFER_H
