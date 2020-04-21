//
// Created by anarion on 4/21/20.
//

#ifndef MYCPPBASE_JSONARRAY_H
#define MYCPPBASE_JSONARRAY_H

#include <container/List/LinkedList.hpp>
#include "JsonObject.h"

namespace anarion {
    class JsonArray : public virtual JsonObject, public LinkedList<JsonObject*> {
    protected:
    public:
        StringBuilder getStringBuilder() override;

        ~JsonArray() override;
    };
}

#endif //MYCPPBASE_JSONARRAY_H
