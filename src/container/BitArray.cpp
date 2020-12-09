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

void anarion::BitArray::resize(anarion::size_type length)  {
    this->length = length;
    size_type arrayLength = length / 8, moreLength = length % 8;
    if (moreLength) { ++arrayLength; }
    arr.resize(arrayLength);
}

bool anarion::BitArray::check(anarion::size_type index) {
    if (index >= length) {
        return false;
    }
    size_type arr_index, bit_index;
    computeIndex(index, &arr_index, &bit_index);
    return arr[arr_index] & (1u << bit_index);
}

void anarion::BitArray::set(anarion::size_type index) {
    size_type arr_index, bit_index;
    computeIndex(index, &arr_index, &bit_index);
    expandIfToSmall(arr_index);
    arr[arr_index] |= 1u << bit_index;
}

void anarion::BitArray::clear(anarion::size_type index) {
    checkOutOfRange(index);
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
        arr.resize((arr_index + 1) * 2);
    }
    for (size_type index = arr.size(); index <= arr_index; ++index) {
        arr.pushBack(0);
    }
    length = arr.size() * segSize;
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
    seg_t *restArray = reinterpret_cast<unsigned char *>(array + arrayLength);
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

void anarion::BitArray::checkOutOfRange(anarion::size_type index) const {
    if (index >= length) {
        throw OutOfRange();
    }
}
