//
// Created by 左钰 on 2020/1/24.
//

#ifndef MYCPPLIB_UNCOPYABLE_H
#define MYCPPLIB_UNCOPYABLE_H


#include "../exceptions/InvalidOperation.h"

namespace anarion {
    class UnCopyable {
    public:
        UnCopyable() = default;

        UnCopyable(const UnCopyable &rhs) { throw InvalidOperation(); }

        UnCopyable(UnCopyable &&rhs) = default;

        UnCopyable &operator=(const UnCopyable &rhs) { throw InvalidOperation(); }

        UnCopyable &operator=(UnCopyable &&rhs) = default;
    };
}

#endif //MYCPPLIB_UNCOPYABLE_H
