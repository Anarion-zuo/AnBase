//
// Created by anarion on 8/19/20.
//

#include <parser/ListParser.h>
#include <io/channel/path/Path.h>
#include <libgen.h>
#include <unistd.h>


anarion::Path::Path(const anarion::SString &pathString)
    : pathString(pathString) {
    /*
    ListParser parser('/');
    pathList = parser.parse(pathString);
    switch (pathString.get(0)) {
        case '/':
            // absolute path
            pathList.remove(pathList.begin_iterator());
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
    return Path(move(parentName));
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
    return Path(move(ret));
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
    return Path(move(cwdString));
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
    return Path(move(newstr));
}
