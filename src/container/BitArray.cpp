//
// Created by anarion on 7/23/20.
//

#include <container/List/BitArray.h>

anarion::BitArray::BitArray(unsigned long length) : length(length) {
    size_type arrayLength = length / 8, moreLength = length % 8;
    if (moreLength) { ++arrayLength; }
    arr.resize(arrayLength);
}

anarion::BitArray::~BitArray() {}

void anarion::BitArray::resize(BitArray::size_type length)  {
    this->length = length;
    size_type arrayLength = length / 8, moreLength = length % 8;
    if (moreLength) { ++arrayLength; }
    arr.resize(arrayLength);
}

bool anarion::BitArray::check(BitArray::size_type index) {
    size_type arr_index, bit_index;
    computeIndex(index, &arr_index, &bit_index);
    return arr[arr_index] & (1u << bit_index);
}

void anarion::BitArray::set(BitArray::size_type index) {
    size_type arr_index, bit_index;
    computeIndex(index, &arr_index, &bit_index);
    expandIfToSmall(arr_index);
    arr[arr_index] |= 1u << bit_index;
}

void anarion::BitArray::clear(BitArray::size_type index) {
    size_type arr_index, bit_index;
    computeIndex(index, &arr_index, &bit_index);
    expandIfToSmall(arr_index);
    arr[arr_index] &= ~(1u << bit_index);
}

void anarion::BitArray::clearAll() {
    bzero(arr.getArr(), arr.size());
}

void anarion::BitArray::setAll() {
    memset(arr.getArr(), 0xff, arr.size());
}

void anarion::BitArray::expandIfToSmall(anarion::size_type arr_index) {
    if (arr_index >= arr.capacity()) {
        arr.resize(arr_index + 1);
    }
    if (arr_index >= arr.size()) {
        arr.insert(arr.end_iterator(), (unsigned char)0, arr_index - arr.size() + 2);
    }
}

bool anarion::BitArray::andAll() const {
    size_type arrayLength = length / (8 * sizeof(size_type)), moreLength = length % (8 * sizeof(size_type));
    size_type *array = reinterpret_cast<size_type *>(arr.getArr());
    for (size_type index = 0; index < arrayLength; ++index) {
        if (~array[index]) {
            return false;
        }
    }
    unsigned char *restArray = reinterpret_cast<unsigned char *>(array + arrayLength);
    unsigned int byteCount = moreLength / 8, bitMore = moreLength % 8;
    for (size_type index = 0; index < byteCount; ++index) {
        if (restArray[index] != 0xff) {
            return false;
        }
    }
    if (bitMore) {
        return restArray[byteCount] & (~(0xffu << bitMore));
    }
    return true;
}

bool anarion::BitArray::orAll() const {
    size_type arrayLength = length / (8 * sizeof(size_type)), moreLength = length % (8 * sizeof(size_type));
    size_type *array = reinterpret_cast<size_type *>(arr.getArr());
    for (size_type index = 0; index < arrayLength; ++index) {
        if (array[index] != 0) {
            return true;
        }
    }
    unsigned char *restArray = reinterpret_cast<unsigned char *>(array + arrayLength);
    unsigned int byteCount = moreLength / 8, bitMore = moreLength % 8;
    for (size_type index = 0; index < byteCount; ++index) {
        if (restArray[index] != 0) {
            return true;
        }
    }
    if (bitMore) {
        return restArray[byteCount] & (0xffu << bitMore);
    }
    return true;
}
