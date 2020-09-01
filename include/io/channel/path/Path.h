//
// Created by anarion on 8/19/20.
//

#ifndef ANBASE_PATH_H
#define ANBASE_PATH_H

#include <container/SString.h>
#include <container/Map/HashMap.hpp>
#include <container/List/LinkedList.hpp>
#include <SystemException.h>

namespace anarion {
class Path {
protected:
    using perm_t = int;
    SString pathString;

    int runAccessSysCall(perm_t perm) const ;
public:
    explicit Path(const SString &pathString);
    explicit Path(SString &&pathString);
    ~Path() = default;
    Path(const Path &) = default;
    Path(Path &&) noexcept = default;
    Path &operator=(const Path &) = default;
    Path &operator=(Path &&) noexcept = default;

    const SString &getString() const ;
    bool isAbsolute() const ;
    bool isValid() const ;
    bool canRead() const ;
    bool canWrite() const ;
    bool canExecute() const ;
    bool isDirectory() const ;

    Path getParent() const ;
    SString getBaseName() const ;
    Path getAbsolute() const ;

    void toAbsolute();

    static Path getCwd();
    static Path combine(const Path &x, const Path &y);
};
struct PathException : public SystemException {};
struct GetCwdFailed : public PathException {};
struct BaseNameFailed : public PathException {};
struct RealPathFailed : public PathException {};
struct DirNameFailed : public PathException {};
}

#endif //ANBASE_PATH_H
