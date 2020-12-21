//
// Created by 左钰 on 2020/1/26.
//

#ifndef MYCPPLIB_STATEMENTINITIALIZATIONFAILED_H
#define MYCPPLIB_STATEMENTINITIALIZATIONFAILED_H


#include "DbcException.h"

class StatementInitializationFailed : public DbcException {
public:
    StatementInitializationFailed(::MYSQL *mysql) : DbcException(mysql) {}
};


#endif //MYCPPLIB_STATEMENTINITIALIZATIONFAILED_H
