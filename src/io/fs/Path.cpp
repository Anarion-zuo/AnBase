//
// Created by anarion on 8/19/20.
//

#include <parser/ListParser.h>
#include <io/fs/Path.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>
#include <io/base/io-exceptions.h>


anarion::Path::Path(const anarion::SString &pathString)
    : pathString(pathString) {
    /*
    ListParser parser('/');
    pathList = parser.parse(pathString);
    switch (pathString.get(0)) {
        case '/':
            // absolute path
            pathList.remove(pathList.beginIterator());
            isAbsolute = true;
            break;
        default:
            // relative path
            break;
    }
     */
}

anarion::Path::Path(anarion::SString &&pathString) : pathString(forward<SString>(pathString)) {}

anarion::Path anarion::Path::getParent() const {
    SString parentName(pathString);
    char *ret = ::dirname(parentName.cstr());
    if (ret == nullptr) {
        throw DirNameFailed();
    }
    parentName.resize(::strlen(parentName.cstr()));
    return Path(anarion::move(parentName));
}

const anarion::SString &anarion::Path::getString() const {
    return pathString;
}

anarion::SString anarion::Path::getBaseName() const {
    const char *baseName = basename(pathString.cstr());
    if (baseName == nullptr) {
        throw BaseNameFailed();
    }
    return SString(baseName);
}

anarion::Path anarion::Path::getAbsolute() const {
    const char *realPath = realpath(pathString.cstr(), nullptr);
    if (realPath == nullptr) {
        throw RealPathFailed();
    }
    SString ret (realPath);
    free((void *) realPath);
    return Path(anarion::move(ret));
}

void anarion::Path::toAbsolute() {
    const char *realPath = realpath(pathString.cstr(), nullptr);
    if (realPath == nullptr) {
        throw RealPathFailed();
    }
    SString newPathString (realPath);
    free((void *) realPath);
    pathString = newPathString;
}

anarion::Path anarion::Path::getCwd() {
    const char *cwd = getcwd(nullptr, 0);
    if (cwd == nullptr) {
        throw GetCwdFailed();
    }
    SString cwdString(cwd);
    free((void *) cwd);
    return Path(anarion::move(cwdString));
}

bool anarion::Path::isAbsolute() const {
    if (pathString.length() == 0) {
        return false;
    }
    return pathString.get(0) == '/';
}

bool anarion::Path::isValid() const {
    int ret = runAccessSysCall(0000);
    if (ret == -1) {
        if (errno == ENOENT) {
            return false;
        }
    }
    return true;
}

int anarion::Path::runAccessSysCall(anarion::Path::perm_t perm) const {
    return access(pathString.cstr(), perm);
}

anarion::Path anarion::Path::combine(const anarion::Path &x, const anarion::Path &y) {
    const SString &xstr = x.pathString, &ystr = y.pathString;
    if (xstr.length() == 0) {
        return y;
    }
    SString newstr(xstr);
    if (xstr.get(xstr.length() - 1) != '/') {
        newstr.append("/", 1);
    }
    newstr.append(ystr);
    return Path(anarion::move(newstr));
}

void anarion::Path::remove() const {
    int ret = ::remove(getString().cstr());
    if (ret < 0) {
        throw RemoveFileFailed();
    }
}

void anarion::Path::move(const anarion::Path &src, const anarion::Path &dst) {
    int ret = ::rename(src.getString().cstr(), dst.getString().cstr());
    if (ret < 0) {
        throw RenameFailed();
    }
}

void anarion::Path::rename(const anarion::Path &src, const anarion::Path &dst) {
    int ret = ::rename(src.getString().cstr(), dst.getString().cstr());
    if (ret < 0) {
        throw RenameFailed();
    }
}

void anarion::Path::symlink(const anarion::Path &src, const anarion::Path &dst) {
    int ret = ::symlink(src.getString().cstr(), dst.getString().cstr());
    if (ret == -1) {
        throw LinkFailed();
    }
}

void anarion::Path::hardlink(const anarion::Path &src, const anarion::Path &dst) {
    int ret = ::link(src.getString().cstr(), dst.getString().cstr());
    if (ret < 0) {
        throw LinkFailed();
    }
}

void anarion::Path::chmod(mode_t mode) {
    int ret = ::chmod(getString().cstr(), mode);
    if (ret < 0) {
        throw ChmodFailed();
    }
}

void anarion::Path::chown(uid_t owner, gid_t group) {
    int ret = ::chown(getString().cstr(), owner, group);
    if (ret < 0) {
        throw ChownFailed();
    }
}

void anarion::Path::mkdir(mode_t mode) {
    int ret = ::mkdir(getString().cstr(), mode);
}
