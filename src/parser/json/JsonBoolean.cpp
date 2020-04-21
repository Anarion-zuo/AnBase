//
// Created by anarion on 4/21/20.
//

#include "parser/json/JsonBoolean.h"

anarion::SString anarion::JsonBoolean::trueString("true"), anarion::JsonBoolean::falseString("false");
anarion::HashMap<anarion::SString, bool> anarion::JsonBoolean::str2boolean {
        {SString("true"), true},
        {SString("false"), false},
};

anarion::StringBuilder anarion::JsonBoolean::getStringBuilder() {
    StringBuilder ret;
    if (boolean) {
        ret.cappend(trueString.getArr(), trueString.length());
        return move(ret);
    }
    ret.cappend(falseString.getArr(), falseString.length());
    return move(ret);
}
