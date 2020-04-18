//
// Created by anarion on 2020/4/2.
//

#include "resource/JsPayload.h"

anarion::SString anarion::JsPayload::defaultContentType("text/css;charset=utf-8");

anarion::JsPayload::JsPayload(anarion::SString &&dir) : FilePayload(forward<SString>(dir)) {

}

const anarion::SString &anarion::JsPayload::getContentType() {
    return defaultContentType;
}
