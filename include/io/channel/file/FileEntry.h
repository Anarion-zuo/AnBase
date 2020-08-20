//
// Created by anarion on 4/6/20.
//

#ifndef MYCPPBASE_FILEENTRY_H
#define MYCPPBASE_FILEENTRY_H

#include <container/SString.h>
#include <container/Map/HashMap.hpp>
#include <container/List/LinkedList.hpp>

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
     *
     * It is resolved that the class structure should look like this:
     *      FileEntry
     *       -- Directory
     *       -- FileChannel
     */

    class FileEntry {
    protected:
        SString name;
        FileEntry *parent = nullptr;
        LinkedList<FileEntry*> childs;
        HashMap<SString, FileEntry*> name2childs;

        // paths
        void computePaths();

        SString computeRelativePath() const ;
        SString computeAbsolutePath() const ;
        SString computeNameSuffix() const ;

        void removeChildFromMembers(FileEntry *entry);
    public:

        // open create close
        virtual void open() = 0;
        virtual void release() = 0;
        virtual void close() = 0;

        // read/write
        virtual size_type in(const char *data, size_type nbytes) = 0;
        virtual size_type in(Buffer &buffer) = 0;
        virtual size_type in(Buffer &buffer, size_type nbytes) = 0;
        virtual size_type out(char *p, size_type nbytes) = 0;
        virtual Buffer out(size_type nbytes) = 0;
        virtual Buffer out() = 0;

        // change operating offset
        virtual void moveForth(size_type steps) = 0;
        virtual void moveBack(size_type steps) = 0;

        // remove/move/rename
        virtual void remove() = 0;
        virtual void move(const SString &newPath) = 0;

        // sync
        virtual void sync() = 0;

        // fetch attributes
        virtual void fetchAttributes() = 0;
        virtual void getAttributes() const = 0;

        // symbolic/hard links
        virtual void symlink(const SString &linkPath) = 0;
        virtual void hardlink(const SString &linkPath) = 0;

        // times
        virtual void getTimeInfo() = 0;

        // permissions
        virtual void changePermission(int perm) = 0;
        virtual bool hasPermission() = 0;

        // sizes
        virtual size_type size() const = 0;

        // entry type
        virtual bool isFile() const { return false; }

        // names
        constexpr const SString &getName() const { return name; }
        constexpr const SString &getSuffix() const { return nameSuffix; }

        // hierarchy
        constexpr FileEntry *getParent() const { return parent; }
        constexpr void setParent(FileEntry *p) { parent = p; }
        FileEntry * getChild(const SString &name);
        constexpr LinkedList<FileEntry*> &getChilds() { return childs; }
        constexpr const HashMap<SString, FileEntry*> &getMap() const { return name2childs; }
        HashMap<SString, FileEntry*> &getMap() { return name2childs; }

        // path
        SString getRelativePath() const { return computeRelativePath(); }
        SString getAbsolutePath() const { return computeAbsolutePath(); }
        FileEntry *findByDir(const SString &dir);

        // ctors & dtors
        explicit FileEntry(SString &&name);
        FileEntry(SString &&name, FileEntry *parent);
        FileEntry(FileEntry &&rhs) noexcept : name(move(rhs.name)), childs(move(rhs.childs)) {}
        virtual ~FileEntry() {}

//        constexpr FileEntry *getParent() const { return parent; }

    };

    template <> struct hash_function<FileEntry*> {
        hash_type operator()(FileEntry *entry) { return hash_function<SString>().operator()(entry->getName()); }
    };
}

#endif //MYCPPBASE_FILEENTRY_H
