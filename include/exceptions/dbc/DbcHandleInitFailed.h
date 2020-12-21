//
// Created by 左钰 on 2020/1/26.
//

#ifndef MYCPPLIB_DBCHANDLEINITFAILED_H
#define MYCPPLIB_DBCHANDLEINITFAILED_H


#include "DbcException.h"

class DbcHandleInitFailed : public DbcException {
public:
    DbcHandleInitFailed(anarion::native_mysql::MYSQL *mysql) : DbcException(mysql) {}
};


#endif //MYCPPLIB_DBCHANDLEINITFAILED_H
