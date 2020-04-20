//
// Created by anarion on 4/17/20.
//

#ifndef MYCPPBASE_SERIALIZABLE_H
#define MYCPPBASE_SERIALIZABLE_H

#include <container/SString.h>

namespace anarion {
    struct serialize_trait {

    };

    class Serializable {
    public:
        virtual SString serialize();
    };
}

#endif //MYCPPBASE_SERIALIZABLE_H
