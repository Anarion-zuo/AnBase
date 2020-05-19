//
// Created by anarion on 4/6/20.
//

#include <parser/ListParser.h>
#include "io/channel/file/FileEntry.h"

anarion::FileEntry::FileEntry(SString &&name) : name(forward<SString>(name)) {
    nameSuffix = this->name.suffix('.');
}

anarion::FileEntry * anarion::FileEntry::getChild(const anarion::SString &name) {
    for (auto it = childs.begin_iterator(); it != childs.end_iterator(); ++it) {
        if ((**it).getName() == name) {
            return *it;
        }
    }
    return nullptr;
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
