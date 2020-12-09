//
// Created by anarion on 2020/1/30.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_BIT_ARRAY_HPP
#define MYCPPLIB_BIT_ARRAY_HPP

#include <cstring>
#include <new>
#include "Vector.hpp"

namespace anarion {
    class BitArray {
        using seg_t = unsigned char;
        const static uint16_t segSize = sizeof(seg_t);
    protected:
        Vector<seg_t> arr;
        size_type length = 0;

        void computeIndex(size_type index, size_type *arr_index, size_type *bit_index) {
            *arr_index = index / segSize;
            *bit_index = index % segSize;
        }

        void expandIfToSmall(size_type arr_index);

        void checkOutOfRange(size_type index) const ;

    public:
        BitArray() = default;
        explicit BitArray(size_type length);
        ~BitArray();

        void resize(size_type length);
        constexpr size_type getLength() const { return length; }
        constexpr unsigned char *getArr() const { return arr.getArr(); }
        constexpr size_type getCharLength() const { return arr.size(); }

        bool check(size_type index);
        void set(size_type index);
        void clear(size_type index);
        void clearAll();
        void setAll();

        bool andAll() const ;
        bool orAll() const ;

    struct Exception : public std::exception {};
    struct OutOfRange : public Exception {};

    };
}

#endif //MYCPPLIB_BIT_ARRAY_HPP
