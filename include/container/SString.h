//
// Created by 左钰 on 2020/1/11.
//

#ifndef MYCPPLIB_SSTRING_H
#define MYCPPLIB_SSTRING_H

#include "List/Vector.hpp"
#include "Set/HashSet.hpp"
#include "base/container_utility.hpp"

namespace anarion {
    class Buffer;
    class SString : public Vector<char> {
        friend class StringBuilder;
        friend class Buffer;

        hash_type hashVal = 1;
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
        static SString move(char *p, size_type len);
        static SString move(Buffer &&buffer);

        bool equals(SString *rhs) const ;
        bool equals(const char *c) const ;
        size_type hash() ;
        hash_type getHashVal() const ;
        size_type length() const;
        char *cstr() const;
        void copy_cstr(char *p) const;
        char *copy_cstr() const;
        void release_copied(char *p) const;
        void append(char *p, size_type num);
        void append(const SString &rhs);
        SString suffix(char dot) const ;   // appointed separator

        size_type indexOf(char c) const ;
        size_type indexOf(char *p, size_type len);
        size_type indexOf(const char *str) const ;  // KMP
        size_type indexOf(const SString &rhs) const ;
        size_type indexOfSince(char c, size_type index);
        size_type indexSkip(const char *str, size_type index);

        void upperCase();
        void lowerCase();

        long toDecSigned() const ;
        size_type toDecUnsigned() const ;
        static SString parseDec(size_type num);
        static SString parseDec(long num);
        static SString parseDec(int num);
        static SString parseHex(unsigned long num);
    };

    template <> struct hash_function<SString> {
        hash_type operator()(const SString &str) { return str.getHashVal(); }
    };
};

#endif //MYCPPLIB_SSTRING_H
