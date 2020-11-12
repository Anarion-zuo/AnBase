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
    /**
     * @details Wrapper on a string representing a path, or a path string. The class methods does not tell among absolute or relative forms directly, but calls on the system library, which in turn may note such distinctions.
     *
     * @details This class wraps some common file system operations.
     */
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

    /**
     * @return whether the string is of absolute path form.
     * @details This method is to simply check whether the string starts with /.
     */
    bool isAbsolute() const ;

    /**
     * @return whether the path string points to an actual node in the file system.
     * @details This is done by trying to open the path with certain permission mode. Upon success, the path is valid.
     */
    bool isValid() const ;
    bool canRead() const ;
    bool canWrite() const ;
    bool canExecute() const ;
    bool isDirectory() const ;

    /**
     * @return the parent directory of this path string.
     * @details This is a directly wrapping of system library function dirname. This string, or the returned string does not guarantee of having the right format.
     */
    Path getParent() const ;
    /**
     * @return the name of the node to which this path string points.
     * @details This is a directly wrapping of system library function basename. This string, or the returned string does not guarantee of having the right format.
     */
    SString getBaseName() const ;
    /**
     * @return the absolute form of the same path.
     * @details This is a directly wrapping of system library function realpath. Only when this string is valid, does this function works.
     */
    Path getAbsolute() const ;

    /**
     * @details Set this string to its own absolute form. This string must be of the right format for this method to succeed.
     */
    void toAbsolute();

    /**
     * @return current working directory of the calling process.
     * @details Direct wrapping of syscall getcwd.
     */
    static Path getCwd();
    /**
     * @details This static method combines 2 path strings into 1, taking each as a part of a path string, as "hi/" and "hello" would be combined into "hi/hello", or "hi" and "hello" would also be combined into "hi/hello".
     * @param x
     * @param y
     * @return the combined path string.
     */
    static Path combine(const Path &x, const Path &y);

    // remove/move/rename
    /**
     * @details Direct wrapping of syscall remove.
     */
    void remove() const;
    /**
     * @details Direct wrapping of syscall rename.
     * @param src source file path.
     * @param dst moving destination.
     */
    static void move(const Path &src, const Path &dst);
    /**
     * @details Direct wrapping of syscall rename.
     * @param src source file path.
     * @param dst new name.
     */
    void rename(const Path &src, const Path &dst);

    // symbolic/hard links
    /**
     * @details Direct wrapping of syscall symlink.
     * @param src path to file to be linked.
     * @param dst link create destination.
     */
    static void symlink(const Path &src, const Path &dst);
    /**
     * @details Direct wrapping of syscall link.
     * @param src path to file to be linked.
     * @param dst link create destination.
     */
    static void hardlink(const Path &src, const Path &dst);

    // permissions
    void chmod(mode_t mode);
    void chown(uid_t owner, gid_t group);

    // create
    void mkdir(mode_t mode);
};
struct PathException : public SystemException {};
struct GetCwdFailed : public PathException {};
struct BaseNameFailed : public PathException {};
struct RealPathFailed : public PathException {};
struct DirNameFailed : public PathException {};
}

#endif //ANBASE_PATH_H
