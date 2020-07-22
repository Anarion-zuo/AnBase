//
// Created by anarion on 4/6/20.
//

#ifndef MYCPPBASE_FILEENTRY_H
#define MYCPPBASE_FILEENTRY_H

#include <container/SString.h>
#include <container/Map/HashMap.hpp>
#include <container/List/LinkedList.hpp>

namespace anarion {

    class FileEntry {
    protected:
        SString name, nameSuffix;
        FileEntry *parent = nullptr;
        LinkedList<FileEntry*> childs;
        HashMap<SString, FileEntry*> name2childs;

        // paths
        SString relativePath, absolutePath;
        void computePaths();
    public:
        void removeChildFromMembers(FileEntry *entry);

        explicit FileEntry(SString &&name);
        FileEntry(SString &&name, FileEntry *parent);
        FileEntry(FileEntry &&rhs) noexcept : name(move(rhs.name)), nameSuffix(move(rhs.nameSuffix)), childs(move(rhs.childs)) {}
        virtual ~FileEntry() {}

        constexpr const SString &getName() const { return name; }
        virtual bool isFile() const { return false; }
        constexpr const SString &getSuffix() const { return nameSuffix; }
        constexpr FileEntry *getParent() const { return parent; }
        constexpr void setParent(FileEntry *p) { parent = p; }
        constexpr const SString &getRelativePath() const { return relativePath; }
        constexpr const SString &getAbsolutePath() const { return absolutePath; }

        FileEntry * getChild(const SString &name);
        constexpr LinkedList<FileEntry*> &getChilds() { return childs; }
        constexpr const HashMap<SString, FileEntry*> &getMap() const { return name2childs; }
        HashMap<SString, FileEntry*> &getMap() { return name2childs; }
//        constexpr FileEntry *getParent() const { return parent; }

        virtual void open() = 0;
        virtual void release() = 0;
        virtual void remove() = 0;
        virtual void close() = 0;

        FileEntry *findByDir(const SString &dir);
    };

    template <> struct hash_function<FileEntry*> {
        hash_type operator()(FileEntry *entry) { return hash_function<SString>().operator()(entry->getName()); }
    };
}

#endif //MYCPPBASE_FILEENTRY_H
