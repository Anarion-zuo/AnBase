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
        LinkedList<FileEntry*> childs;
//        FileEntry *parent;

    public:
        explicit FileEntry(SString &&name);
        FileEntry(FileEntry &&rhs) noexcept : name(move(rhs.name)), nameSuffix(move(rhs.nameSuffix)), childs(move(rhs.childs)) {}
        virtual ~FileEntry() = default;

        constexpr const SString &getName() const { return name; }
        bool isFile() const { return childs.empty(); }
        constexpr const SString &getSuffix() const { return nameSuffix; }

        FileEntry * getChild(const SString &name);
        constexpr LinkedList<FileEntry*> &getChilds() { return childs; }
//        constexpr FileEntry *getParent() const { return parent; }

        virtual void open() = 0;
        virtual void release() = 0;

        FileEntry *findByDir(const SString &dir);
    };

    template <> struct hash_function<FileEntry*> {
        hash_type operator()(FileEntry *entry) { return hash_function<SString>().operator()(entry->getName()); }
    };
}

#endif //MYCPPBASE_FILEENTRY_H
