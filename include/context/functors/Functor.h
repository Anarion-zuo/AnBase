//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_FUNCTOR_H
#define MYCPPLIB_FUNCTOR_H


#include "../Callable.h"

namespace anarion {
    template<typename T>
    class Functor : virtual public Callable {
    protected:
        T ret;
    public:
        inline T getRet() {
            return ret;
        }
    };
}

#endif //MYCPPLIB_FUNCTOR_H
