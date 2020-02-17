//
// Created by anarion on 2020/1/30.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_BIT_ARRAY_HPP
#define MYCPPLIB_BIT_ARRAY_HPP

#include <cstring>

#define BitArrSize (size / 8 + 1)

template <size_type size>
class BitArray {
    typedef unsigned long size_type;
protected:
    char arr[BitArrSize];

    void computeIndex(size_type index, size_type *arr_index, size_type *bit_index) {
        *arr_index = index / 8;
        *bit_index = index % 8;
    }

public:
    BitArray() {}

    bool check(size_type index) {
        size_type arr_index, bit_index;
        computeIndex(index, &arr_index, &bit_index);
        return arr[arr_index] & (1u << bit_index);
    }

    void set(size_type index) {
        size_type arr_index, bit_index;
        computeIndex(index, &arr_index, &bit_index);
        arr[arr_index] |= 1u << bit_index;
    }

    void clear(size_type index) {
        size_type arr_index, bit_index;
        computeIndex(index, &arr_index, &bit_index);
        arr[arr_index] &= ~(1u << bit_index);
    }

    void clearAll() {
        memset(arr, 0, BitArrSize);
    }

    void setAll() {
        memset(arr, 0xff, BitArrSize);
    }

};


#endif //MYCPPLIB_BIT_ARRAY_HPP
