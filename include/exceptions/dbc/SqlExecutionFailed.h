//
// Created by 左钰 on 2020/1/26.
//

#ifndef MYCPPLIB_SQLEXECUTIONFAILED_H
#define MYCPPLIB_SQLEXECUTIONFAILED_H


#include "DbcException.h"

class SqlExecutionFailed : public DbcException {
public:
    SqlExecutionFailed(::MYSQL *mysql) : DbcException(mysql) {}
};


#endif //MYCPPLIB_SQLEXECUTIONFAILED_H
