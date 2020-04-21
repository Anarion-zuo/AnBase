//
// Created by anarion on 4/21/20.
//

#ifndef MYCPPBASE_JSONBOOLEAN_H
#define MYCPPBASE_JSONBOOLEAN_H

#include "JsonObject.h"

namespace anarion {
    class JsonBoolean : public virtual JsonObject {
    protected:
        bool boolean;

        static SString trueString, falseString;
        static HashMap<SString, bool> str2boolean;

    public:
        explicit JsonBoolean(bool boolean) : boolean(boolean) {}

        bool isTrue() const { return boolean; }

        StringBuilder getStringBuilder() override;
    };
}

#endif //MYCPPBASE_JSONBOOLEAN_H
