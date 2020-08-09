//
// Created by anarion on 8/7/20.
//

#include "TestString.h"


static unsigned long MySQLHash(const char *ptr, unsigned long len) {
    unsigned long nr = 1, nr2 = 4;
    for (unsigned long i = 0; i < len; ++i) {
        nr ^= (((nr & 63) + nr2) * ((unsigned int)ptr[i])) + (nr << 8);
        nr2 += 3;
    }
    return nr;
}

void anarion::TestString::runner() {
    SString string("1234567890");
    checkString(string);

    string.insert(1, 'p');
    checkString(string);

    string.insert(7, 'p');
    checkString(string);

    string.append("123456", 6);
    checkString(string);
    if (string.length() != 18) { throwTestFailed(); }

}

void anarion::TestString::checkString(const anarion::SString &string) {
    char *cstr = string.cstr();
    bool equLength = string.length() != strlen(cstr),
        hashEqu = string.getHashVal() != MySQLHash(cstr, string.length()),
        lengthLess = string.length() > string.capacity();
    if (equLength || hashEqu || lengthLess) {
        throwTestFailed();
    }
}
