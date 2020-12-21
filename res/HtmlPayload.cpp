//
// Created by anarion on 2020/4/2.
//

#include "resource/HtmlPayload.h"

anarion::SString anarion::HtmlPayload::defaultContentType("text/html;charset=utf-8");

anarion::HtmlPayload::HtmlPayload(anarion::SString &&dir) : FilePayload(forward<SString>(dir)) {}

const anarion::SString &anarion::HtmlPayload::getContentType() {
    return defaultContentType;
}
