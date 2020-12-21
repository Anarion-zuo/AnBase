//
// Created by 左钰 on 2020/1/24.
//

#ifndef MYCPPLIB_INVALIDOPERATION_H
#define MYCPPLIB_INVALIDOPERATION_H


#include "MyException.h"

class InvalidOperation : public MyException {
public:
    const char *what() const noexcept override {
        return "This operation is not recommended therefore strictly forbidden. This exception is for bypassing compiling warnings yet ban a few attempts.";
    }
};


#endif //MYCPPLIB_INVALIDOPERATION_H
