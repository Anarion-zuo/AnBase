//
// Created by 左钰 on 2020/1/11.
//

#ifndef MYCPPLIB_SSTRING_H
#define MYCPPLIB_SSTRING_H

#include "List/Vector.hpp"
#include "Set/HashSet.hpp"
#include "base/container_utility.hpp"

namespace anarion {

    class SString;
    /*
     * Interface for mappable classes
     * For classes to be serialized to and parsed from strings conveniently
     */
    class MapperInterface {
    public:
        MapperInterface() = default;
        explicit MapperInterface(const SString &str) {}
        virtual SString serialize() = 0;
        virtual SString forwardSerialize() = 0;
    };

    class Buffer;
    class SString : protected Vector<char>, public virtual MapperInterface {
        friend class StringBuilder;
        friend class Buffer;

        mutable hash_type hashVal = 1;
        mutable bool needsHash = true;
    public:

        SString();
        explicit SString(const char *str);

        SString serialize() override;
        SString forwardSerialize() override;

        ~SString() override ;

        SString(char *p, size_type num);
        SString(SString &&rhs) noexcept;
        SString(const SString &rhs);

        SString &operator=(const SString &rhs);
        SString &operator=(SString &&rhs) noexcept;

        bool operator==(const SString &rhs) const ;
        bool operator!=(const SString &rhs) const ;

//        static SString move(const char *str);
        static SString move(char *p, size_type len);
//        static SString move(Buffer &&buffer);

        bool empty() const ;
        size_type capacity() const ;
        size_type length() const;
        char *getArr() const;
        void resize(size_type newSize);

        char &get(size_type index);
        const char &get(size_type index) const ;
        char &operator[](size_type index);
        const char &operator[](size_type index) const ;

        bool equals(SString *rhs) const ;
        bool equals(const char *c) const ;
        size_type hash() const ;
        size_type hash(size_type index, size_type len) const ;
        hash_type getHashVal() const ;
        char *cstr() const;
        void copy_cstr(char *p) const;
        char *copy_cstr() const;
        void release_copied(char *p) const;
        void append(char *p, size_type num);
        void append(const SString &rhs);
        SString suffix(char dot) const ;   // appointed separator

//        size_type indexOf(char c) const ;
//        size_type indexOf(char *p, size_type len);
//        size_type indexOf(const char *str) const ;  // KMP
//        size_type indexOf(const SString &rhs) const ;
//        size_type indexOfSince(char c, size_type index);
//        size_type indexSkip(const char *str, size_type index);

        void insert(size_type index, char c);
        void insert(size_type index, const char *p, size_type num);
        void push_back(char c);

        void upperCase();
        void lowerCase();

        long toDecSigned() const ;
        size_type toDecUnsigned() const ;
        static SString parseDec(size_type num);
        static SString parseDec(long num);
        static SString parseDec(int num);
        static size_type decIntegerBitCount(size_type num);
        static SString parseDec(double num, int floatBitCount);
        static SString parseDec(double num);
        static SString parseHex(unsigned long num);

        static SString concat(const Vector<SString> &strings);
    };

    template <> struct hash_function<SString> {
        hash_type operator()(const SString &str) { return str.getHashVal(); }
    };
    template <> struct hash_function<SString*> {
        hash_type operator()(const SString *str) { return str->getHashVal(); }
    };
};

#endif //MYCPPLIB_SSTRING_H
