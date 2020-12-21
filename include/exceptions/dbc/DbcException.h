//
// Created by 左钰 on 2020/1/26.
//

#ifndef MYCPPLIB_DBCEXCEPTION_H
#define MYCPPLIB_DBCEXCEPTION_H


#include "../MyException.h"
#include "../../dbc/nativedbc.h"

class DbcException : virtual public MyException {
protected:
    ::MYSQL *mysql;
public:
    DbcException(::MYSQL *mysql) : mysql(mysql) {}

    const char *what() const noexcept override {
        return ::mysql_error(mysql);
    }
};


#endif //MYCPPLIB_DBCEXCEPTION_H
