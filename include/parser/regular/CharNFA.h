//
// Created by anarion on 11/18/20.
//

#ifndef ANBASE_CHARNFA_H
#define ANBASE_CHARNFA_H

#include <container/List/Vector.hpp>
#include <container/Map/HashMap.hpp>
#include <concurrent/base/Mutex.h>

namespace anarion {
class CharNFA {
    struct Node {
        HashMap<char, Vector<Node*>> char2nexts;

        Vector<Node *> &getNext(char c);

        static Node *destination, *error;
        static Mutex mutex;
        static Node *getDestination();

        bool isDestination() const ;
        size_type nextsCount() const ;

        static Node *parseRegular(const char *regular, size_type length);
    };

    Node *source = nullptr;

public:
    CharNFA(const char *regular, size_type length);

    bool match(const char *expr, size_type length);
};
}

#endif //ANBASE_CHARNFA_H
