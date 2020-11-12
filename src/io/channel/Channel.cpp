//
// Created by anarion on 2020/2/20.
//

#include "io/channel/Channel.h"
//
// Created by anarion on 2020/2/21.
//

#include "io/base/sys_utility.h"
#include <io/base/io-exceptions.h>


using namespace anarion;

void Channel::throwInvalidOperation(const char *functionName) {
    throw InvalidIoOperations(functionName);
}
