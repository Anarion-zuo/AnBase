//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_LOCALCONTEXT_H
#define MYCPPLIB_LOCALCONTEXT_H


#include "Context.h"
#include "../container/Map/HashMap.hpp"

namespace anarion {
    class LocalContext : virtual public Context {
    protected:
        int pid;   // actual process identifier
//        static hash_map<int, LocalContext> *pid2context;
    public:
        void wait() override;

        void fork();   // fork and execute
        void stop();   // kill the process
        int getPid() const;

//        LocalContext & getParent();
    };
}

#endif //MYCPPLIB_LOCALCONTEXT_H
