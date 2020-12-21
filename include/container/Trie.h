//
// Created by anarion on 5/14/20.
//

#ifndef ANBASE_TRIE_H
#define ANBASE_TRIE_H

#include <container/Map/HashMap.hpp>
#include "SString.h"

namespace anarion {
    class Trie {
    protected:
        struct Node {
            // initialized for convenience
            size_type passedCount = 1, endCount = 0;
            HashMap<char, Node*> childs;

            Node() = default;
            explicit Node(size_type passedCount) : passedCount(passedCount) {}  // for rootNode initialization

            Node *getChild(char c);
            void removeChilds();
            void addChild(char c, Node *node);
            void removeChild(char c);
        };

        Node *root = new Node(0);

    public:
        Trie() = default;
        ~Trie() {
            root->removeChilds();
            delete root;
        }

        void addWord(const SString &word, size_type offset = 0);
        size_type wordCount(const SString &word, size_type offset = 0) const ;
        void removeWord(const SString &word, size_type offset = 0, size_type count = 1);
        void addWord(const char *str, size_type length);
        size_type wordCount(const char *str, size_type length) const ;
        void removeWord(const char *str, size_type length, size_type count = 1);

        size_type longestPrefix(const SString &str, size_type offset = 0) const ;
        size_type longestPrefix(const char *str, size_type length) const ;
    };

    struct TrieException : public std::exception {};
    struct TrieRemovingTooMany : public TrieException {
        const char *what() const noexcept override {
            return "Removing the word for too many times in a trie";
        }
    };
}

#endif //ANBASE_TRIE_H
