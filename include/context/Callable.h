//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_CALLABLE_H
#define MYCPPLIB_CALLABLE_H


namespace anarion {
    class Callable {
    public:
        virtual void run() {}
        virtual Callable *clone() const { return new Callable(); }
    };
}

#endif //MYCPPLIB_CALLABLE_H
