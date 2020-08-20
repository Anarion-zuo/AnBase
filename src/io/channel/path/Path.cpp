//
// Created by anarion on 8/19/20.
//

#include <parser/ListParser.h>
#include "io/channel/path/Path.h"

anarion::Path::Path(const anarion::SString &pathString) {
    ListParser parser('/');
    pathList = parser.parse(pathString);
    switch (pathString.get(0)) {
        case '/':
            // absolute path
            pathList.remove(pathList.begin_iterator());
            break;
        default:
            // relative path
            break;
    }
}

void anarion::Path::printPath() const {
    for (auto it = pathList.begin_iterator(); it != pathList.end_iterator(); ++it) {
        printf("#%s# ", it->cstr());
    }
    printf("\n");
}
