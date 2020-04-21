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
            char *p = nullptr;
            size_type len = 0;

            builder_n() = default;
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
        StringBuilder(StringBuilder &&rhs) noexcept : list(move(rhs.list)) {}
        ~StringBuilder() = default;

        void cappend(char *p, size_type len);
        void cappend(const SString &str) { cappend(str.getArr(), str.length()); }
        void cappend(const char *p) { cappend(const_cast<char*>(p), strlen(p)); }
        void mappend(char *p, size_type len);
        void mappend(SString &&str);
        void appendBuilder(StringBuilder &&rhs);

        SString build() ;
    };
}

#endif //MYCPPBASE_STRINGBUILDER_H
