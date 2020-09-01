//
// Created by anarion on 4/6/20.
//

#include <parser/ListParser.h>
#include <container/StringBuilder.h>
#include <unistd.h>
#include <exceptions/InvalidOperation.h>
#include "io/channel/file/FileEntry.h"

anarion::FileEntry::FileEntry(const SString &name) : path(name) {}
/*
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
*/
void anarion::FileEntry::throwInvalidOperation() {
    throw InvalidIOParameters();
}

anarion::FileEntry::FileEntry(anarion::FileEntry &&rhs) noexcept
    : path(anarion::move(rhs.path)) {

}

anarion::FileEntry::FileEntry(anarion::Path &&path)
    : path(forward<Path>(path)) {

}

