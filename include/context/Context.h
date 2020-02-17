// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_CONTEXT_H
#define MYCPPLIB_CONTEXT_H

#include "Callable.h"
#include <exception>

namespace anarion {
    class Context : virtual public Callable {
    protected:
    public:

        Context();  // initialize a context for the current process

        virtual void preRun();  // for specifying initializing terms
        virtual void postRun();

        virtual void onException(std::exception &e);

        virtual void wait();  // dont wait for yourself
        void execute();
    };
}

#endif //MYCPPLIB_CONTEXT_H
