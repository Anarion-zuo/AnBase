//
// Created by anarion on 2020/4/2.
//

#include "resource/CssPayload.h"

anarion::SString anarion::CssPayload::defaultContentType("text/css;charset=utf-8");

anarion::CssPayload::CssPayload(anarion::SString &&dir) : FilePayload(forward<SString>(dir)) {

}

const anarion::SString &anarion::CssPayload::getContentType() {
    return defaultContentType;
}
