//
// Created by anarion on 2020/1/30.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_BIT_ARRAY_HPP
#define MYCPPLIB_BIT_ARRAY_HPP

#include <cstring>


class BitArray {
    typedef unsigned long size_type;
protected:
    unsigned char *arr = nullptr;
    size_type length = 0;

    void computeIndex(size_type index, size_type *arr_index, size_type *bit_index) {
        *arr_index = index / 8;
        *bit_index = index % 8;
    }

public:
    BitArray() = default;

    BitArray(size_type length) : length(length) {
        size_type arrayLength = length / 8, moreLength = length % 8;
        if (moreLength) { ++arrayLength; }
        arr = static_cast<unsigned char *>(operator new(arrayLength));
        bzero(arr, arrayLength);
    }

    ~BitArray() {
        size_type arrayLength = length / 8, moreLength = length % 8;
        if (moreLength) { ++arrayLength; }
        operator delete (arr, arrayLength);
    }

    void setLength(size_type length) {
        if (!(this->length == 0 && arr == nullptr)) {
            throw "BitArray Length already set";
        }
        this->length = length;
        size_type arrayLength = length / 8, moreLength = length % 8;
        if (moreLength) { ++arrayLength; }
        arr = static_cast<unsigned char *>(operator new(arrayLength));
        bzero(arr, arrayLength);
    }

    constexpr size_type getLength() const { return length; }

    constexpr unsigned char *getArr() const { return arr; }
    constexpr size_type getCharLength() const {
        size_type arrayLength = length / 8, moreLength = length % 8;
        if (moreLength) {
            ++arrayLength;
        }
        return arrayLength;
    }

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
        size_type arrayLength = length / 8, moreLength = length % 8;
        if (moreLength) { ++arrayLength; }
        bzero(arr, arrayLength);
    }

    void setAll() {
        size_type arrayLength = length / 8, moreLength = length % 8;
        if (moreLength) { ++arrayLength; }
        memset(arr, 0xff, arrayLength);
    }

};


#endif //MYCPPLIB_BIT_ARRAY_HPP
