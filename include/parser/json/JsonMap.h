//
// Created by anarion on 4/21/20.
//

#ifndef MYCPPBASE_JSONMAP_H
#define MYCPPBASE_JSONMAP_H

#include "JsonObject.h"

namespace anarion {
    class JsonMap : public JsonObject, public HashMap<SString, JsonObject*> {
    protected:
        StringBuilder makeKey(const SString &key);

    public:
        ~JsonMap() override ;

        StringBuilder getStringBuilder() override;
    };
}

#endif //MYCPPBASE_JSONMAP_H
