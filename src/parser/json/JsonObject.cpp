//
// Created by anarion on 4/21/20.
//

#include "parser/json/JsonObject.h"

namespace anarion {
    JsonMap *parseJson(const char *expression, size_type length);
}

anarion::JsonMap *anarion::JsonObject::parse(const char *expression, anarion::size_type length) {
    return anarion::parseJson(expression, length);
}

anarion::JsonMap *anarion::JsonObject::parse(const anarion::SString &expression) {
    return anarion::parseJson(expression.getArr(), expression.length());
}
