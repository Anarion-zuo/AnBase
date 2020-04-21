//
// Created by anarion on 4/21/20.
//

#ifndef MYCPPBASE_JSONNUMBER_H
#define MYCPPBASE_JSONNUMBER_H

#include "JsonObject.h"

namespace anarion {
    class JsonNumber : virtual public JsonObject {
        long number;
    public:
        explicit JsonNumber(long number) : number(number) {}

        long getNumber() const { return number; }

        StringBuilder getStringBuilder() override {
            StringBuilder ret;
            ret.cappend(SString::parseDec(number));
            return move(ret);
        }
    };
}

#endif //MYCPPBASE_JSONNUMBER_H
