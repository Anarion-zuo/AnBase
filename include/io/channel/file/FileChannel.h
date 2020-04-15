#ifndef FILECHANNEL_H
#define FLIECHANNEL_H

#include <io/buffer/FixedBuffer.h>
#include "../RandomChannel.h"
#include "container/SString.h"
#include "FileEntry.h"

namespace anarion {
class FileChannel : virtual public RandomChannel, public virtual FileEntry {
protected:
    int fd;

    explicit FileChannel(SString &&name, int fd) : FileEntry(forward<SString>(name)), Channel(), InChannel(true), OutChannel(true),
                                                   RandomChannel(true), fd(fd) {}

public:

    FileChannel(FileChannel &&rhs) noexcept : Channel(forward<FileChannel>(rhs)), InChannel(forward<FileChannel>(rhs)), OutChannel(forward<FileChannel>(rhs)), RandomChannel(forward<FileChannel>(rhs)), fd(rhs.fd), FileEntry(forward<FileChannel>(rhs)) {}
    ~FileChannel() { if (valid()) { close(); } }

    static FileChannel open(const SString &dir);
    void release() override;
    void close() override;

    size_type in(char *p, size_type nbytes) override;
    size_type in(Buffer &buffer) override;
    size_type in(Buffer &buffer, size_type nbytes) override;
    size_type out(char *p, size_type nbytes) override;
    Buffer out(size_type nbytes) override;
    Buffer out() override ;

    size_type in(FixedBuffer &buffer);
    size_type in(FixedBuffer &buffer, size_type nbytes);
    FixedBuffer outBuffer(size_type nbytes);
    FixedBuffer outBuffer();

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