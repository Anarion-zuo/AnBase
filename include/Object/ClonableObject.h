//
// Created by anarion on 2020/3/5.
//

#ifndef MYCPPBASE_CLONABLEOBJECT_H
#define MYCPPBASE_CLONABLEOBJECT_H

#include "RootObject.h"

namespace anarion {
    class ClonableObject : virtual public RootObject {
    public:
        virtual ClonableObject *clone() const { return new ClonableObject(*this); }
    };
}

#endif //MYCPPBASE_CLONABLEOBJECT_H
