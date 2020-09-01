//
// Created by anarion on 4/6/20.
//

#ifndef MYCPPBASE_FILEENTRY_H
#define MYCPPBASE_FILEENTRY_H

#include <container/SString.h>
#include <container/Map/HashMap.hpp>
#include <container/List/LinkedList.hpp>
#include <io/base/io-exceptions.h>
#include <io/channel/path/Path.h>
#include <time/Time.h>

namespace anarion {

    /*
     * This is the class manipulating the file system.
     *
     * The file system is organized mainly as a tree structure,
     *      therefore its manipulation must follow this pattern.
     * Leaves may be occupied by various kinds of files, not only regular files,
     *      whilst upper nodes must be occupied by directories.
     *      The file system provides poor structured directory manipulation API,
     *      not similar to regular files at all, and we have to live with it.
     *
     * Entry provides all possible operations for files,
     *      although some of them may not be valid to certain types of entry.
     *      The operations are as following
     *
     *  - open/create/close
     *  - read/write
     *  - change operating offset
     *  - remove/move/rename
     *  - sync
     *  - fetch attributes
     *  - symbolic/hard links
     *  - times
     *  - permissions
     *  - sizes
     *
     * For directories, things are a bit special.
     *     - Directories must support traversing, namely, get child directories and files.
     *     - Directories can not be read or written like a regular file.
     * Afterall, directories are stored as inodes, same as regular files.
     * The class Directory is inherited from class FileChannel, so that it operates on similar file system interface.
     * The stream read/write operations of directories should be prevented at runtime.
     * This implementation uses dirent utilities from Unix interface.
     *
     * There are 2 ways of making use of this file system utility class and other io utilities.
     *      You either use *Channel classes independently,
     *      or compose *Channel classes with Directory, thus uses this hierarchy.
     */

    class FileEntry {
    protected:
        using perm_t = int;
        using flag_t = int;
        /*
         * Path and naming is a headache.
         * An entry needs not store the full path name of it self.
         * Its parent contains the `parent` part of the path.
         * It is decreed that at the top of the file management structure,
         *      the root node should store a path indexing it self,
         *      whilst the lower nodes should store only their own names.
         *
         * This does not solve the problem entirely,
         *      for when the process cwd changes, the relative paths must change accordingly,
         *      or the path becomes invalid.
         */
        Path path;

        static void throwInvalidOperation();
    public:

        // open create close
        virtual void open() = 0;
        virtual void close() = 0;
        virtual void create(perm_t perm) = 0;
        virtual bool isOpen() const = 0;

        // read/write
        virtual size_type in(const char *data, size_type nbytes) = 0;
        virtual size_type in(Buffer &buffer) = 0;
        virtual size_type in(Buffer &buffer, size_type nbytes) = 0;
        virtual size_type out(char *p, size_type nbytes) = 0;
        virtual Buffer out(size_type nbytes) = 0;
        virtual Buffer out() = 0;

        // change operating offset
        virtual void move_forth(size_type steps) = 0;
        virtual void move_back(size_type steps) = 0;
        virtual void set_cursor(size_type index) = 0;

        // remove/move/rename
        virtual void remove() = 0;
        virtual void move(const SString &newPath) = 0;

        // sync
        virtual void sync() = 0;

        // fetch attributes
        virtual void fetchAttributes() = 0;
        virtual const struct stat & getAttributes() const = 0;

        // symbolic/hard links
        virtual void symlink(const SString &linkPath) = 0;
        virtual void hardlink(const SString &linkPath) = 0;

        // times
        virtual Time getLastAccessTime() = 0;
        virtual Time getLastModifyTime() = 0;
        virtual Time getLastStatusChangeTime() = 0;

        // permissions
        virtual void changePermission(int perm) = 0;
        virtual bool hasPermission(int perm) = 0;

        // sizes
        virtual size_type size() const = 0;

        // entry type
        virtual bool isFile() const { return false; }

        // names
        constexpr const Path &getPath() const { return path; }
        SString getSuffix() const { return path.getString().suffix('.'); }
        virtual void rename(const SString &newName) = 0;

        // path
        SString getAbsolutePath() const { return path.getAbsolute().getString(); }

        // ctors & dtors
        explicit FileEntry(const SString &name);
        FileEntry(FileEntry &&rhs) noexcept ;
        virtual ~FileEntry() = default ;
        FileEntry(Path &&path);

    };

    template <> struct hash_function<FileEntry*> {
        hash_type operator()(FileEntry *entry) { return hash_function<SString>().operator()(entry->getAbsolutePath()); }
    };
}

#endif //MYCPPBASE_FILEENTRY_H
