//
// Created by anarion on 2020/2/28.
//

#include "container/StringBuilder.h"

void anarion::StringBuilder::cappend(char *p, anarion::size_type len) {
    char *np = static_cast<char *>(operator new(len));
    memcpy(np, p, len);
    list.push_back(builder_n(np, len));
}

void anarion::StringBuilder::mappend(char *p, anarion::size_type len) {
    list.push_back(builder_n(p, len));
}

anarion::SString anarion::StringBuilder::build() {
    // count byte number
    size_type total_len = 0;
    for (auto it = list.begin_iterator(); it != list.end_iterator(); ++it) {
        total_len += it->len;
    }
    if (total_len == 0) {
        return SString();
    }
    // no copy required
    if (list.size() == 1) {
        SString ret;
        builder_n &n = *list.begin_iterator();
        ret.begin = n.p;
        ret.cur = ret.begin + n.len;
        ret.end = ret.cur;
        n.len = 0;
        n.p = nullptr;
        ret.hash();
        return move(ret);
    }
    // copy
    char *p = static_cast<char *>(operator new(total_len)), *pp = p;
    for (auto it = list.begin_iterator(); it != list.end_iterator(); ++it) {
        memcpy(p, it->p, it->len);
        p += it->len;
    }
    // inject string object
    SString ret;
    ret.begin = pp;
    ret.end = pp + total_len;
    ret.cur = ret.end;
    ret.hash();
    return move(ret);
}

void anarion::StringBuilder::appendBuilder(anarion::StringBuilder &&rhs) {
    list.push_back(move(rhs.list));
}

void anarion::StringBuilder::mappend(anarion::SString &&str) {
    char *cstr = str.getArr();
    size_type len = str.length();
    str.clearMove();
    mappend(cstr, len);
}

void anarion::StringBuilder::appendNumber(anarion::size_type num) {
    mappend(SString::parseDec(num));
}

void anarion::StringBuilder::reverseOrder() {
    list.reverseOrder();
}
