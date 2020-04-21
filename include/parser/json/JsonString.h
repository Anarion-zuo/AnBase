//
// Created by anarion on 4/21/20.
//

#ifndef MYCPPBASE_JSONSTRING_H
#define MYCPPBASE_JSONSTRING_H

#include "JsonObject.h"

namespace anarion {
    class JsonString : virtual public JsonObject, virtual public SString {
    public:
        StringBuilder getStringBuilder() override {
            StringBuilder ret;
            ret.cappend(*this);
            return anarion::move(ret);
        }

        explicit JsonString(SString &&rhs) noexcept : SString(forward<SString>(rhs)) {}
    };
}

#endif //MYCPPBASE_JSONSTRING_H
