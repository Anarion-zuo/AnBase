//
// Created by 左钰 on 2020/1/26.
//

#ifndef MYCPPLIB_EXTRACTRESULTFAILED_H
#define MYCPPLIB_EXTRACTRESULTFAILED_H

#include "DbcException.h"

class ExtractResultFailed : public DbcException {
public:
    ExtractResultFailed(::MYSQL *mysql) : DbcException(mysql) {}
};


#endif //MYCPPLIB_EXTRACTRESULTFAILED_H
