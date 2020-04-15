//
// Created by anarion on 4/6/20.
//

#ifndef MYCPPBASE_LISTPARSER_H
#define MYCPPBASE_LISTPARSER_H

#include <container/List/LinkedList.hpp>
#include <container/SString.h>

namespace anarion {
    class ListParser {
    protected:
        char separator;

    public:
        explicit ListParser(char separator) : separator(separator) {}
        LinkedList<SString> parse(const SString &expression);
    };
}

#endif //MYCPPBASE_LISTPARSER_H
