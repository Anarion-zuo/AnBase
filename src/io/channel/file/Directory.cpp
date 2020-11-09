//
// Created by anarion on 4/6/20.
//

#include <unistd.h>
#include <io/channel/file/FileChannel.h>
#include <container/StringBuilder.h>
#include <sys/stat.h>
#include "io/channel/file/Directory.h"

static bool isDots(const char *str) {
    if (*str != '.') {
        return false;
    }
    if (str[1] != '.') {
        if (str[1] == '\0') {
            return true;
        }
        return false;
    }
    if (str[2] != '\0') {
        return false;
    }
    return true;
}

anarion::Directory::Directory(const SString &name)
    : FileChannel(name) {}

anarion::Directory::Directory(anarion::Directory &&rhs) noexcept
        : FileChannel(forward<Directory>(rhs)), dirp(rhs.dirp), dirEnt(rhs.dirEnt) {
    rhs.dirp = nullptr;
    rhs.dirEnt = nullptr;
}

/*
anarion::FileEntry *anarion::Directory::createChildFile(const SString &fileName) {
    StringBuilder builder;
    builder.cappend(relativePath);
    builder.cappend(fileName);
    SString createName = builder.build();
    FileEntry *entry = new FileChannel(createName);
    childs.push_back(entry);
    name2childs.insert({fileName, entry});
    return entry;
}

anarion::FileEntry *anarion::Directory::createChildDirectory(anarion::SString &&dirName) {
    StringBuilder absoluteName;
    absoluteName.cappend(relativePath);
    absoluteName.cappend(dirName);
    absoluteName.cappend("/", 1);
    SString absoluteNewName = absoluteName.build();
    char *p = absoluteNewName.copy_cstr();
    int ret = mkdir(p, 0777);
    absoluteNewName.release_copied(p);
    if (ret < 0) {
        throw DirectoryCreateException();
    }
    FileEntry *entry = new Directory(SString(dirName));
    entry->open();
    childs.push_back(entry);
    name2childs.insert({forward<SString>(dirName), entry});
    return entry;
}
*/

#pragma region invalid_operations
anarion::size_type anarion::Directory::in(const char *data, anarion::size_type nbytes) {
    throwInvalidOperation();
}

anarion::size_type anarion::Directory::in(anarion::Buffer &buffer) {
    throwInvalidOperation();
}

anarion::size_type anarion::Directory::in(anarion::Buffer &buffer, anarion::size_type nbytes) {
    throwInvalidOperation();
}

anarion::size_type anarion::Directory::out(char *p, anarion::size_type nbytes) {
    throwInvalidOperation();
}

anarion::Buffer anarion::Directory::out(anarion::size_type nbytes) {
    throwInvalidOperation();
}

anarion::Buffer anarion::Directory::out() {
    throwInvalidOperation();
}

void anarion::Directory::move_forth(anarion::size_type steps) {
    throwInvalidOperation();
}

void anarion::Directory::move_back(anarion::size_type steps) {
    throwInvalidOperation();
}

void anarion::Directory::set_cursor(anarion::size_type index) {
    throwInvalidOperation();
}
#pragma endregion

void anarion::Directory::open() {
    dirp = opendir(path.getString().cstr());
    if (dirp == nullptr) {
        throw DirectoryCreateFailed();
    }
    fd = dirfd(dirp);
    if (dirp == nullptr) {
        throw DirectoryOpenFailed();
    }
    fetchAttributes();
}

void anarion::Directory::create(perm_t perm) {
    int ret = ::mkdir(path.getString().cstr(), perm);
    if (ret < 0) {
        throw DirectoryCreateFailed();
    }
}

void anarion::Directory::remove() {
    int ret = ::rmdir(path.getString().cstr());
    if (ret < 0) {
        throw RemoveDirectoryFailed();
    }
    fd = -1;
    dirEnt = nullptr;
    dirp = nullptr;
}

void anarion::Directory::close() {
    if (!isOpen()) {
        return;
    }
    // not closing fd directly
    ::closedir(dirp);
    dirp = nullptr;
    dirEnt = nullptr;
    fd = -1;
}

anarion::Directory::~Directory() {
    if (fd < 0) {
        return;
    }
    // not closing fd directly
    ::closedir(dirp);
    dirp = nullptr;
    dirEnt = nullptr;
    fd = -1;
}

struct dirent *anarion::Directory::next() const {
    moveNextEnt();
    return dirEnt;
}

void anarion::Directory::rewindIterate() const {
    if (dirEnt == nullptr) {
        dirEnt = readdir(dirp);
    }
    ::rewinddir(dirp);
    dirEnt = nullptr;
}

anarion::size_type anarion::Directory::currentIndex() const {
    long ret = ::telldir(dirp);
    if (ret < 0) {
        throw TellDirFailed();
    }
    return ret;
}

void anarion::Directory::setIndex(size_type index) {
    ::seekdir(dirp, index);
}

struct dirent *anarion::Directory::curEnt() const {
    if (dirEnt == nullptr) {
        moveNextEnt();
    }
    return dirEnt;
}

anarion::size_type anarion::Directory::size() const {
    rewindIterate();
    do {

    } while (next());
}

void anarion::Directory::moveNextEnt() const {
    errno = 0;
    do {
        dirEnt = ::readdir(dirp);
        if (dirEnt == nullptr) {
            if (errno) {
                throw DirectoryIterateFailed();
            }
            break;
        }
    } while (isDots(dirEnt->d_name));
}

anarion::FileChannel anarion::Directory::direntToFile(flag_t oflags) const {
    if (curEnt()->d_type != DT_REG) {
        throw DirEntryToFileFailed();
    }
    Path newPath = Path::combine(path, Path(SString(curEnt()->d_name)));
    int newFd = ::open(newPath.getString().cstr(), oflags);
    if (newFd < 0) {
        throw OpenFdFailed();
    }
    FileChannel file(anarion::move(newPath), oflags);
    file.setFd(newFd);
    return anarion::move(file);
}

/*
anarion::Directory::iterator::iterator(int fd)
        : fd(fd), dirp(fdopendir(fd)), ent(readdir(dirp)) {

}

anarion::Directory::iterator::~iterator() {

}

dirent &anarion::Directory::iterator::operator*() {
    return *ent;
}


const dirent &anarion::Directory::iterator::operator*() const {
    return *ent;
}

dirent *anarion::Directory::iterator::operator->() const {
    return ent;
}

anarion::Directory::iterator &anarion::Directory::iterator::operator++() {
    ent = readdir(dirp);
    return *this;
}

const anarion::Directory::iterator anarion::Directory::iterator::operator++(int) {
    iterator result(*this);
    ++(*this);
    return result;
}

bool anarion::Directory::iterator::operator==(const iterator &rhs) const {
    return dirp == rhs.dirp &&
           ent == rhs.ent;
}

bool anarion::Directory::iterator::operator!=(const iterator &rhs) const {
    return !(rhs == *this);
}
*/
