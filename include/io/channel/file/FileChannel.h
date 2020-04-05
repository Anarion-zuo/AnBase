#ifndef FILECHANNEL_H
#define FLIECHANNEL_H

#include "../RandomChannel.h"
#include "container/SString.h"

namespace anarion {
class FileChannel : virtual public RandomChannel {
protected:
    int fd;

    explicit FileChannel(int fd) : Channel(), InChannel(true), OutChannel(true), RandomChannel(true), fd(fd) {}

public:

    FileChannel(FileChannel &&rhs) noexcept : Channel(forward<FileChannel>(rhs)), InChannel(forward<FileChannel>(rhs)), OutChannel(forward<FileChannel>(rhs)), RandomChannel(forward<FileChannel>(rhs)), fd(rhs.fd) {}
    ~FileChannel() { if (valid()) { close(); } }

    static FileChannel open(const SString &dir);
    void close() override;

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
};
}

#endif // FILECHANNEL_H