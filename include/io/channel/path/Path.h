//
// Created by anarion on 8/19/20.
//

#ifndef ANBASE_PATH_H
#define ANBASE_PATH_H

#include <container/SString.h>
#include <container/Map/HashMap.hpp>
#include <container/List/LinkedList.hpp>

namespace anarion {
class Path {
protected:
    LinkedList<SString> pathList;
public:
    explicit Path(const SString &pathString);
    ~Path() = default;

    void printPath() const ;
};
}

#endif //ANBASE_PATH_H
