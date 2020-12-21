//
// Created by anarion on 5/14/20.
//

#include "container/Trie.h"

anarion::Trie::Node *anarion::Trie::Node::getChild(char c) {
    auto it = childs.find(c);
    if (it == childs.end_iterator()) { return nullptr; }
    return it->get_val();
}

void anarion::Trie::Node::removeChilds() {
    for (auto it = childs.begin_iterator(); it != childs.end_iterator(); ++it) {
        it->get_val()->removeChilds();
        delete it->get_val();
    }
}

void anarion::Trie::Node::addChild(char c, anarion::Trie::Node *node) {
    childs.put(c, node);
}

void anarion::Trie::Node::removeChild(char c) {
    childs.remove(c);
}

void anarion::Trie::addWord(const char *str, anarion::size_type length) {
    Node *node = root;

    for (size_type index = 0; index < length; ++index) {
        char curChar = str[index];
        ++node->passedCount;

        Node *child = node->getChild(curChar);
        if (child == nullptr) {
            // node not present
            child = new Node;
            node->addChild(curChar, child);
        } else {
            // node present
            // do nothing
        }
        node = child;
    }

    ++node->passedCount;
    ++node->endCount;
}

anarion::size_type anarion::Trie::wordCount(const char *str, anarion::size_type length) const {
    if (root->passedCount == 0) { return 0; }
    Node *node = root;

    for (size_type index = 0; index < length; ++index) {
        char curChar = str[index];
        Node *child = node->getChild(curChar);
        if (child == nullptr) {
            return 0;
        }
        node = child;
    }

    return node->endCount;
}

void anarion::Trie::removeWord(const char *str, anarion::size_type length, anarion::size_type count) {
    size_type presentCount = wordCount(str, length);
    if (count > presentCount) {
        throw TrieRemovingTooMany();
    }

    char curChar = str[0];
    Node *parent = root, *node = root->getChild(curChar);
    --root->passedCount;
    if (root->passedCount == 0) {
        root->removeChilds();
    }

    for (size_type index = 1; index < length; ++index) {
        curChar = str[index];
        --node->passedCount;
        if (node->passedCount == 0) {
            node->removeChilds();
            delete node;
            parent->removeChild(curChar);
            return;
        }
        Node *child = node->getChild(curChar);
        parent = node;
        node = child;
    }
    --node->passedCount;
    --node->endCount;
    if (node->passedCount == 0) {
        delete node;
        parent->removeChild(curChar);
    }
}

void anarion::Trie::addWord(const anarion::SString &word, anarion::size_type offset) {
    addWord(word.cstr() + offset, word.length() - offset);
}

anarion::size_type anarion::Trie::wordCount(const anarion::SString &word, anarion::size_type offset) const {
    wordCount(word.cstr() + offset, word.length() - offset);
}

void anarion::Trie::removeWord(const anarion::SString &word, anarion::size_type offset, anarion::size_type count) {
    removeWord(word.cstr() + offset, word.length() - offset, count);
}

anarion::size_type anarion::Trie::longestPrefix(const anarion::SString &str, anarion::size_type offset) const {
    return longestPrefix(str.cstr() + offset, str.length() - offset);
}

anarion::size_type anarion::Trie::longestPrefix(const char *str, anarion::size_type length) const {
    if (root->passedCount == 0) { return 0; }
    Node *node = root;

    for (size_type index = 0; index < length; ++index) {
        char curChar = str[index];
        Node *child = node->getChild(curChar);
        if (child == nullptr) {
            return index;
        }
        node = child;
    }

    return length;
}
