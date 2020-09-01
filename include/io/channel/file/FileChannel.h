#ifndef FILECHANNEL_H
#define FILECHANNEL_H

#include <io/buffer/Buffer.h>
#include <sys/file.h>
#include "../RandomChannel.h"
#include "container/SString.h"
#include "FileEntry.h"

namespace anarion {
class FileChannel : public RandomChannel, public FileEntry {
protected:
    int fd = -1;
    struct stat statInfo;
    flag_t oflags;

public:

    FileChannel(anarion::Path &&path, flag_t oflags);
    FileChannel(FileChannel &&rhs) noexcept ;
    explicit FileChannel(const SString &dir);
    FileChannel(SString &&name, int fd, flag_t oflags = O_RDWR | O_CREAT);
    FileChannel(const SString &dir, int oflag);
    FileChannel(const SString &dir, int oflag, int perm);
    ~FileChannel() override ;

    static FileChannel open(const SString &dir);
    // open create close
    void open() override;
    void open(perm_t perm);
    void create(perm_t perm) override ;
    void close() override;
    bool isOpen() const override ;

    int getFd() const { return fd; }
    void setFd(int _fd) { fd = _fd; }

    // read/write
    size_type in(const char *p, size_type nbytes) override;
    size_type in(Buffer &buffer) override;
    size_type in(Buffer &buffer, size_type nbytes) override;
    size_type out(char *p, size_type nbytes) override;
    Buffer out(size_type nbytes) override;
    Buffer out() override ;

    // change operating offset
    void rewind() override;
    void set_append() override;
    void move_forth(size_type nbytes) override;
    void move_back(size_type nbytes) override;
    void set_cursor(size_type index) override ;

    // remove/move/rename
    void remove() override ;
    void move(const SString &newPath) override ;
    void rename(const SString &newName) override ;

    // sync
    void sync() override ;

    // fetch attributes
    void fetchAttributes() override ;
    const stat &getAttributes() const override ;

    // symbolic/hard links
    void symlink(const SString &linkPath) override ;
    void hardlink(const SString &linkPath) override ;

    // times
    Time getLastAccessTime() override ;
    Time getLastModifyTime() override ;
    Time getLastStatusChangeTime() override ;

    // permissions
    void changePermission(int perm) override ;
    bool hasPermission(perm_t perm) override ;

    // sizes
    size_type size() const override ;

    // entry type
    bool isFile() const override { return true; }};
}

#endif // FILECHANNEL_H