//
// Created by 左钰 on 2020/4/5.
//

#ifndef MYCPPBASE_MAPPARSER_H
#define MYCPPBASE_MAPPARSER_H

/*
 * The parser takes 2 characters as input and a hash map as output.
 * @Param equal
 *      The equal sign in the input expression.
 * @Param separator
 *      The sign separating the 2 lines in input expression.
 * The parameters are initialized at construct time.
 * The input expression should look like:
 *      max=100; timeout=59
 *      username=anarion ;passwd =hahahaha
 */

#include <container/Map/HashMap.hpp>
#include <container/SString.h>

namespace anarion {
    class MapParser {
    protected:

        char equal, separator;

    public:
        MapParser(char equal, char separator);

        HashMap<SString, SString> parse(const SString &inputExpression);

    };
}

#endif //MYCPPBASE_MAPPARSER_H
