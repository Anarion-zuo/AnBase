//
// Created by 左钰 on 2020/1/10.
//

#ifndef MYCPPLIB_INDEXOUTOFRANGE_H
#define MYCPPLIB_INDEXOUTOFRANGE_H


#include "../MyException.h"

struct IndexOutOfRange : public MyException {
    const char *what() const noexcept {
        return "Index out of range...";
    }
};


#endif //MYCPPLIB_INDEXOUTOFRANGE_H
