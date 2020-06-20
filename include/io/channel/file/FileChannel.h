#ifndef FILECHANNEL_H
#define FLIECHANNEL_H

#include <io/buffer/Buffer.h>
#include "../RandomChannel.h"
#include "container/SString.h"
#include "FileEntry.h"

namespace anarion {
class FileChannel : virtual public RandomChannel, public virtual FileEntry {
protected:
    int fd;
    size_type fileSize = 0;

    void updateSize() {
        if (!valid()) { throw InvalidOperation(); }
        off_t off = ::lseek(fd, 0, SEEK_CUR);
        size_type ret = ::lseek(fd, 0, SEEK_END);
        ::lseek(fd, off, SEEK_SET);
        fileSize = ret;
    }

    explicit FileChannel(SString &&name, int fd) : FileEntry(forward<SString>(name)), Channel(), InChannel(true), OutChannel(true), RandomChannel(true), fd(fd) { updateSize(); }

public:

    FileChannel(FileChannel &&rhs) noexcept : InChannel(forward<FileChannel>(rhs)), OutChannel(forward<FileChannel>(rhs)), RandomChannel(forward<FileChannel>(rhs)), fd(rhs.fd), FileEntry(forward<FileChannel>(rhs)) {
        return;
    }
    ~FileChannel() { if (valid()) { close(); } }

    static FileChannel open(const SString &dir);
    void release() override;
    void close() override;

    int getFd() const { return fd; }

    size_type in(char *p, size_type nbytes) override;
    size_type in(Buffer &buffer) override;
    size_type in(Buffer &buffer, size_type nbytes) override;
    size_type out(char *p, size_type nbytes) override;
    Buffer out(size_type nbytes) override;
    Buffer out() override ;

    void rewind() override;
    void set_append() override;
    void move_forth(size_type nbytes) override;
    void move_back(size_type nbytes) override;
    size_type size() const override;

    bool modifiedLaterThan(const timespec &time);

    void open() override;
};
}

#endif // FILECHANNEL_H