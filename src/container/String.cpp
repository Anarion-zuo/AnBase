//
// Created by 左钰 on 2020/1/11.
//

#include <cstring>
#include "SString.h"
#include "exceptions/container/IndexOutOfRange.h"

using namespace anarion;

char *SString::cstr() const {
    return begin;
}

size_type SString::length() const {
    return size();
}

SString::SString() {
}

SString::~SString() {
    // memory released by Vector destructor
}

SString::SString(char *p, size_type num) : Vector<char>(num) {
    insert(cur, p, num);
}

SString SString::move(const char *str) {
    SString s;
    s.begin = const_cast<char *>(str);
    size_type len = ::strlen(str);
    s.cur = s.begin + len;
    s.end = s.cur;
    return anarion::move(s);
}

void SString::append(char *p, size_type num) {
    insert(end_iterator(), p, p + num);
}

void SString::append(const SString &rhs) {
    append(rhs.begin, rhs.length());
}

SString::SString(const char *str) {
    size_type len = ::strlen(str);
    begin = (char*)operator new(len);
    end = begin + len;
    cur = end;
    memcpy(begin, str, len);
}

bool SString::equals(const char *c) const{
    size_type len = strlen(c);
    if (len < length()) {
        return false;
    }
    for (size_type i = 0; i < length(); ++i) {
        if (begin[i] != c[i]) {
            return false;
        }
    }
    return true;
}

bool SString::equals(SString *rhs) const {
    if (rhs->length() != length()) {
        return false;
    }
    return equals(rhs->cstr());
}


static hash_type MySQLHash(const char *ptr, size_type len) {
    unsigned int nr = 1, nr2 = 4;
    hash_type res = 0;
    for (unsigned long i = 0; i < len; ++i) {
        nr ^= (((nr & 63) + nr2) * ((unsigned int)ptr[i])) + (nr << 8);
        nr2 += 3;
    }
    return nr;
}

SString::SString(SString &&rhs) noexcept : Vector<char>(std::forward<SString>(rhs)) {
}

SString SString::suffix(char dot) const {
    char *p = cur;
    --p;
    // search for dot
    size_type index = 0;
    while (*p != dot) {
        ++index;
        --p;
    }
    if (index == 0) {
        return SString();
    }
    return SString(p + 1, index);
}

void SString::copy_cstr(char *p) const {
    memcpy(p, begin, length());
    *(p + length()) = 0;
}

char *SString::copy_cstr() const {
//    char *p = newObjects<char>(length() + 1);
    char *p = (char*)operator new(length() + 1);
    copy_cstr(p);
    return p;
}

void SString::release_copied(char *p) const {
    operator delete(p, length());
}

SString::SString(const SString &rhs) : Vector<char>(rhs) {

}

SString &SString::operator=(const SString &rhs) {
    if (&rhs == this) {
        return *this;
    }
    Vector<char>::operator=(rhs);
    return *this;
}

SString &SString::operator=(SString &&rhs) noexcept {
    Vector<char>::operator=(std::forward<SString>(rhs));
    return *this;
}

bool SString::operator==(const SString &rhs) const {
    if (rhs.length() != length()) {
        return false;
    }
    return equals(rhs.cstr());
}

bool SString::operator!=(const SString &rhs) const {
    return !(*this == rhs);
}

size_type SString::hash() const {
    return MySQLHash(begin, length());
}

static bool is_lower(char c) { return c <= 'z' && c >= 'a'; }
static bool is_upper(char c) { return c <= 'Z' && c >= 'A'; }

static void cstr_upper(char *p, size_type len) {
    for (size_type i = 0; i < len; ++i) {
        if (is_lower(p[i])) { p[i] += 'A' - 'a'; }
    }
}

static void cstr_lower(char *p, size_type len) {
    for (size_type i = 0; i < len; ++i) {
        if (is_upper(p[i])) { p[i] += 'a' - 'A'; }
    }
}

void SString::upperCase() { cstr_upper(begin, size()); }

void SString::lowerCase() { cstr_lower(begin, size()); }
