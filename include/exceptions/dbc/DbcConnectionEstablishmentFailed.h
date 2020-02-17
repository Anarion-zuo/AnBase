//
// Created by 左钰 on 2020/1/26.
//

#ifndef MYCPPLIB_DBCCONNECTIONESTABLISHMENTFAILED_H
#define MYCPPLIB_DBCCONNECTIONESTABLISHMENTFAILED_H

#include "DbcException.h"

class DbcConnectionEstablishmentFailed : public DbcException {
public:
    DbcConnectionEstablishmentFailed(::MYSQL *mysql) : DbcException(mysql) {}
};


#endif //MYCPPLIB_DBCCONNECTIONESTABLISHMENTFAILED_H
