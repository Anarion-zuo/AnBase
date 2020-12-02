//
// Created by 左钰 on 2020/1/11.
//

#include <cstring>
#include <container/SString.h>
#include <io/buffer/Buffer.h>

using namespace anarion;

char *SString::cstr() const {
    return begin;
}

size_type SString::length() const {
    if (Vector<char>::empty()) {
        return 0;
    }
    return size() - 1;
}

SString::SString() : Vector<char>(1) {
    Vector<char>::pushBack(0);
}

SString::~SString() {
    // memory released by Vector destructor
}

SString::SString(char *p, size_type num) : Vector<char>(num + 1) {
    Vector<char>::insert(cur, p, num);
    if (cur[-1] != 0) {
        *cur = 0;
        ++cur;
    }
}

SString SString::move(char *p, size_type len) {
    SString ret;
    if (len == 0) {
        return anarion::move(ret);
    }
    if (p[len - 1] == 0) {
        operator delete (ret.begin, 1);
        ret.begin = p;
        ret.cur = p + len;
        ret.end = ret.cur;
        return anarion::move(ret);
    }
    return SString(p, len);
}

//SString SString::move(Buffer &&buffer) {
//    size_type len = buffer.unread();
//    char *p = static_cast<char *>(operator new(len));
//    buffer.write_arr(p, len);
//    return SString::move(p, len);
//}

void SString::append(char *p, size_type num) {
    Vector<char>::insert(cur - 1, p, p + num);
    needsHash = true;
}

void SString::append(const SString &rhs) {
    append(rhs.begin, rhs.length());
}

SString::SString(const char *str) {
    size_type len = ::strlen(str);
    begin = (char*)operator new(len + 1);
    end = begin + len + 1;
    cur = end;
    memcpy(begin, str, len);
    cur[-1] = 0;
    needsHash = true;
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
    return equals(rhs->cstr());
}


SString::SString(SString &&rhs) noexcept
    : Vector<char>(anarion::forward<SString>(rhs)), hashVal(rhs.hashVal), needsHash(rhs.needsHash) {
    rhs.Vector<char>::pushBack(0);
    rhs.hash();
    rhs.needsHash = false;
}

SString SString::suffix(char dot) const {
    if (empty()) {
        return SString();
    }
    char *p = cur;
    --p;
    // search for dot
    size_type index = 0;
    while (*p != dot) {
        ++index;
        --p;
        if (p == begin) {
            if (*p != dot) {
                return SString();
            }
        }
    }
    if (index == 0) {
        return SString();
    }
    if (index >= size()) {
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

SString::SString(const SString &rhs) : Vector<char>(rhs), hashVal(rhs.hashVal), needsHash(rhs.needsHash) {

}

SString &SString::operator=(const SString &rhs) {
    if (&rhs == this) {
        return *this;
    }
    Vector<char>::operator=(rhs);
    hashVal = rhs.hashVal;
    needsHash = rhs.needsHash;
    return *this;
}

SString &SString::operator=(SString &&rhs) noexcept {
    Vector<char>::operator=(anarion::forward<SString>(rhs));
    hashVal = rhs.hashVal;
    needsHash = rhs.needsHash;
    rhs.hash();
    rhs.needsHash = false;
    return *this;
}

bool SString::operator==(const SString &rhs) const {
    if (rhs.length() != length()) {
        return false;
    }
//    return equals(rhs);
    const char *rhs_cstr = rhs.cstr();
    for (size_type i = 0; i < length(); ++i) {
        if (begin[i] != rhs_cstr[i]) {
            return false;
        }
    }
    return true;
}

bool SString::operator!=(const SString &rhs) const {
    return !(*this == rhs);
}

hash_type SString::hash() const {
    if (needsHash) {
        hashVal = hash(0, length());
        needsHash = false;
    }
    return hashVal;
}

size_type SString::hash(size_type index, size_type len) const {
    unsigned long nr = 1, nr2 = 4;
    for (unsigned long i = 0; i < len; ++i) {
        nr ^= (((nr & 63) + nr2) * ((unsigned int)(begin + index)[i])) + (nr << 8);
        nr2 += 3;
    }
    return nr;
}

hash_type SString::getHashVal() const {
    return hash();
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

void SString::upperCase() { cstr_upper(begin, length()); needsHash = true; }

void SString::lowerCase() { cstr_lower(begin, length()); needsHash = true; }

long SString::toDecSigned() const {
    size_type index = length() - 1;
    long ret = 0, tens = 1;
    for (; index > 0; --index) {
        ret += (begin[index] - ('0' - 0)) * tens;
        tens *= 10;
    }
    if (*begin == '-') {
        return -ret;
    }
    return ret + *begin * tens;
}

size_type SString::toDecUnsigned() const {
    size_type index = length() - 1;
    long ret = 0, tens = 1;
    for (; index > 0; --index) {
        ret += (begin[index] - ('0' - 0)) * tens;
        tens *= 10;
    }
    return ret + *begin * tens;
}

SString SString::parseDec(size_type num) {
    char *s = static_cast<char *>(operator new(21));
    int len = sprintf(s, "%lu", num);
    SString ret;
    ret.begin = s;
    ret.cur = s + len;
    ret.end = s + 22;
    ret.hash();
    *ret.cur = '\0';
    ret.cur++;
    return ::move(ret);
}

SString SString::parseDec(long num) {
    char *s = static_cast<char *>(operator new(21));
    int len = sprintf(s, "%ld", num);
    SString ret;
    ret.begin = s;
    ret.cur = s + len;
    ret.end = s + 22;
    ret.hash();
    *ret.cur = '\0';
    ret.cur++;
    return ::move(ret);
}

SString SString::parseDec(int num) {
    char *s = static_cast<char *>(operator new(21));
    int len = sprintf(s, "%d", num);
    SString ret;
    ret.begin = s;
    ret.cur = s + len;
    ret.end = s + 22;
    ret.hash();
    *ret.cur = '\0';
    ret.cur++;
    return ::move(ret);
}

//size_type SString::indexOf(char c) const {
//    for (char *p = begin; p < cur - 1; ++p) {
//        if (*p == c) { return p - begin; }
//    }
//    return cur - begin;
//}
//
//size_type SString::indexOf(const char *str) const {
//    return 0;
//}
//
//size_type SString::indexOf(const SString &rhs) const {
//    return 0;
//}
//
//size_type SString::indexOf(char *p, size_type len) {
//    // most fit length
//
//}
//
//size_type SString::indexOfSince(char c, size_type index) {
//    char *p = begin + index;
//    for (; p < cur; ++p) {
//        if (*p == c) {
//            return p - begin;
//        }
//    }
//    return cur - begin;
//}

inline static bool cstr_hasc(const char *str, char c) {
    while (*str) {
        if (*str == c) {
            return true;
        }
        ++str;
    }
    return false;
}

//size_type SString::indexSkip(const char *str, size_type index) {
//    char *p = begin + index;
//    for (; p < cur; ++p) {
//        if (!cstr_hasc(str, *p)) {
//            return p - begin;
//        }
//    }
//    return cur - begin;
//}

SString SString::parseHex(unsigned long num) {
    char str[18];
    sprintf(str, "%lx", num);
    return SString(str);
}

SString SString::serialize() {
    return *this;
}

SString SString::forwardSerialize() {
    return anarion::move(*this);
}

bool SString::empty() const {
    return Vector<char>::size() == 1;
}

size_type SString::capacity() const {
    return Vector<char>::capacity();
}

void SString::push_back(char c) {
    insert(length(), c);
}

void SString::insert(size_type index, char c) {
    if (index > length()) {
        throw IndexOutOfRange();
    }
    Vector<char>::insert(index, c);
    needsHash = true;
}

void SString::insert(size_type index, const char *p, size_type num) {
    Vector::insert(index, p, num);
    needsHash = true;
}

char &SString::get(size_type index) {
    return Vector::get(index);
}

const char &SString::get(size_type index) const {
    return Vector::get(index);
}

char &SString::operator[](size_type index) {
    return get(index);
}

const char &SString::operator[](size_type index) const {
    return get(index);
}

char *SString::getArr() const {
    return cstr();
}

void SString::resize(size_type newSize) {
    // +1 for the ending 0
    Vector<char>::resize(newSize + 1);
}


