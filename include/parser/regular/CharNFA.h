//
// Created by anarion on 11/18/20.
//

#ifndef ANBASE_CHARNFA_H
#define ANBASE_CHARNFA_H

#include <container/List/Vector.hpp>
#include "NFANode.h"

namespace anarion {
class CharNFA {


    struct MatchInfo {
        LinkedList<NFANode *> nodes;

        bool putChar(char c);
        void init(NFANode *source);

        explicit MatchInfo(NFANode *source) { init(source); }

        void matchFirst();
    };

    NFANode *source = nullptr, *destination = nullptr;

public:
    CharNFA(const char *regular, size_type length);

    bool match(const char *expr, size_type length);
};
}

#endif //ANBASE_CHARNFA_H
