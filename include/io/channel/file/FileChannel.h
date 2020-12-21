#ifndef FILECHANNEL_H
#define FILECHANNEL_H

#include <io/buffer/Buffer.h>
#include <sys/file.h>
#include <sys/stat.h>
#include "FileAttr.h"
#include "../RandomChannel.h"
#include "container/SString.h"
#include "FileEntry.h"
#include "FileOpenFlag.h"

namespace anarion {

/**
 * @details
 * This class is soley devoted to operations on file descriptors.
 * Other operations on files may be provided by the file system. These are not provided by this class if it does not operate on a file descriptor.
 *
 * @details
 * According to APUE, the UNIX syscalls on a file descriptor includes the following and no more:
 * open creat close lseek read write dup sync fcntl ioctl
 */
class FileChannel : public RandomChannel/*, public FileEntry*/ {
    using flag_t = int;
protected:
    int fd = -1;
    FileOpenFlag oflags;
    Path path;

    void checkIsOpen() const ;

    int callFcntl(int cmd) const;
    int callFcntl(int cmd, int arg) const;


public:

    FileChannel(anarion::Path &&path, flag_t oflags);
    FileChannel(Path &&path, FileOpenFlag oflags);

    /**
     * @details Direct wrapping of syscall open.
     * @param dir File directory
     * @param oflag Open flag
     * @details Optional param mode is ommitted.
     */
    FileChannel(const SString &dir, int oflag);

    /**
     * @details This destructor does not make syscall close, which must be called explicitly by the user. This destructor release the object's attributes, if there is any to be released.
     */
    ~FileChannel() override;

    FileChannel(FileChannel &&rhs) noexcept ;

    // open create close
    /**
     * @details Direct wrapping of syscall open, taking params from attributes initialized in constructors. Do not use this for file creation, instead of another method create.
     */
    void open() override;
    void open(mode_t mode);

    /**
     * @details Direct wrapping of syscall create.
     * @param mode
     */
    void create(mode_t mode);

    /**
     * @details Direct wrapping of syscall close. This method is not called by the destructor, therefore must be called explicitly.
     */
    void close() override;

    bool isOpen() const;

    /**
     * @return Current value of file descriptor
     */
    int getFd() const { return fd; }

    /**
     * @param _fd Sets file descriptor value
     */
    void setFd(int _fd) { fd = _fd; }

    Path &getPath() { return path; }
    const Path &getPath() const { return path; }

    // read/write
    size_type in(const char *p, size_type nbytes) override;
    size_type out(char *p, size_type nbytes) override;

    // change operating offset
    void resetCursor() override;
    void setCursorAppend() override;
    void moveForthCursor(size_type nbytes) override;
    void moveBackCursor(size_type nbytes) override;
    void setCursor(size_type index) override;

    // sync
    /**
     * @details Direct wrapping of syscall fsync
     */
    void sync();

    // fetch attributes
    FileAttr getAttributes() const ;

    // sizes
    size_type size() const override;

    // entry type
    //bool isFile() const override { return true; }};

    // fcntl
};
}

#endif // FILECHANNEL_H
