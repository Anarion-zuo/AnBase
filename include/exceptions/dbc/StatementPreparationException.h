//
// Created by 左钰 on 2020/1/26.
//

#ifndef MYCPPLIB_STATEMENTPREPARATIONEXCEPTION_H
#define MYCPPLIB_STATEMENTPREPARATIONEXCEPTION_H


#include "DbcException.h"

class StatementPreparationException : public DbcException {
public:
    StatementPreparationException(::MYSQL *mysql) : DbcException(mysql) {}
};


#endif //MYCPPLIB_STATEMENTPREPARATIONEXCEPTION_H
