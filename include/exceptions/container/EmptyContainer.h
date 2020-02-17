//
// Created by 左钰 on 2020/1/10.
//

#ifndef MYCPPLIB_EMPTYCONTAINER_H
#define MYCPPLIB_EMPTYCONTAINER_H


#include "../MyException.h"

struct EmptyContainer : public MyException {
    const char *what() const noexcept override {
        return "The container has no element...";
    }
};


#endif //MYCPPLIB_EMPTYCONTAINER_H
