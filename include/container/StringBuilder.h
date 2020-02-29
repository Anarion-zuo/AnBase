//
// Created by anarion on 2020/2/28.
//

#ifndef MYCPPBASE_STRINGBUILDER_H
#define MYCPPBASE_STRINGBUILDER_H

#include <container/base/container_utility.hpp>
#include <container/List/LinkedList.hpp>
#include <new>
#include "SString.h"

namespace anarion {
    class StringBuilder {
    protected:
        struct builder_n {
            char *p;
            size_type len;

            builder_n(char *p, size_type len) : p(p), len(len) {}   // constructor for move

            builder_n(const builder_n &rhs) : p(static_cast<char *>(operator new(rhs.len))), len(rhs.len) {
                memcpy(p, rhs.p, len);
            }

            builder_n(builder_n &&rhs) noexcept : p(rhs.p), len(rhs.len) {
                rhs.p = nullptr;
                rhs.len = 0;
            }

            ~builder_n() { operator delete(this->p, len); }
        };

        LinkedList<builder_n> list;

    public:
        StringBuilder() = default;
        ~StringBuilder() = default;

        void cappend(char *p, size_type len);
        void mappend(char *p, size_type len);

        SString build() ;
    };
}

#endif //MYCPPBASE_STRINGBUILDER_H
