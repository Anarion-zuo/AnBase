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
        SString name;
        LinkedList<FileEntry*> childs;
//        FileEntry *parent;

    public:
        explicit FileEntry(SString &&name);
        FileEntry(FileEntry &&rhs) noexcept : name(move(rhs.name)), childs(move(rhs.childs)) {}

        constexpr const SString &getName() const { return name; }
        bool isFile() const { return childs.empty(); }

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
