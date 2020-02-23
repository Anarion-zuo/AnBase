//
// Created by 左钰 on 2020/1/11.
//

#ifndef MYCPPLIB_SSTRING_H
#define MYCPPLIB_SSTRING_H

#include "List/Vector.hpp"
#include "Set/HashSet.hpp"
#include "base/container_utility.hpp"

namespace anarion {
    class SString : public Vector<char> {
    public:
        SString();
        explicit SString(const char *str);

        ~SString();

        SString(char *p, size_type num);
        SString(SString &&rhs) noexcept;
        SString(const SString &rhs);

        SString &operator=(const SString &rhs);
        SString &operator=(SString &&rhs) noexcept;

        bool operator==(const SString &rhs) const ;
        bool operator!=(const SString &rhs) const ;

        static SString move(const char *str);

        bool equals(SString *rhs) const ;
        bool equals(const char *c) const ;
        size_type hash() const ;
        size_type length() const;
        char *cstr() const;
        void copy_cstr(char *p) const;
        char *copy_cstr() const;
        void release_copied(char *p) const;
        void append(char *p, size_type num);
        void append(const SString &rhs);
        SString suffix(char dot) const ;   // appointed separator

        void upperCase();
        void lowerCase();
    };

    template <> struct hash_function<SString> {
        hash_type operator()(const SString &str) { return str.hash(); }
    };
};

#endif //MYCPPLIB_SSTRING_H
