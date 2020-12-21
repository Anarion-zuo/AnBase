//
// Created by anarion on 4/21/20.
//

#ifndef MYCPPBASE_JSONOBJECT_H
#define MYCPPBASE_JSONOBJECT_H

#include <container/Map/HashMap.hpp>
#include <container/SString.h>
#include <container/StringBuilder.h>

namespace anarion {
    class JsonMap;
    class JsonObject {
    protected:
    public:
        static JsonMap *parse(const char *expression, size_type length);
        static JsonMap *parse(const SString &expression);

        virtual ~JsonObject() = default;

        virtual StringBuilder getStringBuilder() = 0;

        SString toString() { return getStringBuilder().build(); }
    };
}

#endif //MYCPPBASE_JSONOBJECT_H
