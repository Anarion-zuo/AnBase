//
// Created by anarion on 4/6/20.
//

#include <parser/ListParser.h>
#include <container/StringBuilder.h>
#include <zconf.h>
#include "io/channel/file/FileEntry.h"

anarion::FileEntry::FileEntry(SString &&name) : name(forward<SString>(name)) {
    nameSuffix = this->name.suffix('.');

    computePaths();
}

anarion::FileEntry::FileEntry(anarion::SString &&name, anarion::FileEntry *parent)
        : name(forward<SString>(name)), parent(parent) {
    nameSuffix = this->name.suffix('.');

    computePaths();
}

anarion::FileEntry * anarion::FileEntry::getChild(const anarion::SString &name) {
    auto it = name2childs.find(name);
    if (it == name2childs.end_iterator()) {
        return nullptr;
    }
    return it->get_val();
}

anarion::FileEntry *anarion::FileEntry::findByDir(const anarion::SString &dir) {
    ListParser parser('/');
    LinkedList<SString> structure = parser.parse(dir);
    if (structure.empty()) {
        return nullptr;
    }
    auto beginIt = structure.begin_iterator();
    if (beginIt->empty()) { // for seemingly absolute path case
        ++beginIt;
    }
    FileEntry *entry = this;
    for (auto it = beginIt; it != structure.end_iterator(); ++it) {
        FileEntry *next = entry->getChild(*it);
        if (next == nullptr) {
            return nullptr;
        }
        entry = next;
    }
    return entry;
}

void anarion::FileEntry::computePaths() {
    FileEntry *entry = parent;
    StringBuilder builder;
    builder.cappend(name);
    while (entry) {
        builder.cappend(entry->getName());
    }
    builder.reverseOrder();
    relativePath = builder.build();
    builder.reverseOrder();
    if (relativePath[0] == '/') {
        absolutePath = relativePath;
    } else {
        const char *cwd = getcwd(nullptr, 0);
        builder.cappend(cwd);
        ::free((void *) cwd);
        builder.reverseOrder();
        absolutePath = builder.build();
    }
}

void anarion::FileEntry::removeChildFromMembers(anarion::FileEntry *entry) {
    getChilds().remove(entry);
    getMap().remove(entry->name);
}

