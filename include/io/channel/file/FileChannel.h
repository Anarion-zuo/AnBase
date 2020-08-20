#ifndef FILECHANNEL_H
#define FILECHANNEL_H

#include <io/buffer/Buffer.h>
#include <sys/file.h>
#include <io/base/io-exceptions.h>
#include "../RandomChannel.h"
#include "container/SString.h"
#include "FileEntry.h"

namespace anarion {
class FileChannel : public RandomChannel, public FileEntry {
protected:
    int fd;
    struct stat statInfo;

    /*
     * calls systemcall open.
     * packs the call and throwing exceptions.
     * should only be called by constructors.
     */
    void initOpen(const char *cdir, int oflag, int perm);

    explicit FileChannel(SString &&name, int fd) : FileEntry(forward<SString>(name)), Channel(), InChannel(true), OutChannel(true), RandomChannel(true), fd(fd) {}

public:

    FileChannel(FileChannel &&rhs) noexcept : InChannel(forward<FileChannel>(rhs)), OutChannel(forward<FileChannel>(rhs)), RandomChannel(forward<FileChannel>(rhs)), FileEntry(forward<FileChannel>(rhs)) , fd(rhs.fd) {
        rhs.fd = -1;
    }
    explicit FileChannel(const SString &dir);
    FileChannel(const SString &dir, int oflag);
    FileChannel(const SString &dir, int oflag, int perm);
    ~FileChannel() override {
        if (valid()) {
            set_invalid();
            ::close(fd);
        }
    }

    static FileChannel open(const SString &dir);
    void release() override;
    void close() override;
    void remove() override ;

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

    bool isFile() const override;

    void open() override;
};
}

#endif // FILECHANNEL_H