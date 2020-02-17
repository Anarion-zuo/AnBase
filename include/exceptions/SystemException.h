//
// Created by 左钰 on 2020/1/11.
//

#ifndef MYCPPLIB_SYSTEMEXCEPTION_H
#define MYCPPLIB_SYSTEMEXCEPTION_H


#include <cerrno>
#include <cstring>
#include "MyException.h"

struct SystemException : public MyException {
    const char *what() const noexcept {
        return strerror(errno);
    }
};


#endif //MYCPPLIB_SYSTEMEXCEPTION_H
