//
// Created by anarion on 12/15/20.
//

#ifndef ANBASE_POOLEXECUTABLE_H
#define ANBASE_POOLEXECUTABLE_H

#include <container/List/Vector.hpp>
#include <container/List/Array.h>

namespace anarion {
    class PoolExecutable {
    public:
        virtual void run() = 0;
    };

    template <typename Callable>
    class CallablePoolExecutable : public PoolExecutable {
        Callable callable;
    public:
        explicit CallablePoolExecutable(const Callable &callable) : callable(callable) {}
        explicit CallablePoolExecutable(Callable &&callable) : callable(forward<Callable>(callable)) {}
        CallablePoolExecutable(CallablePoolExecutable &&rhs) noexcept : callable(move(rhs.callable)) {}
        void run() override {
            callable();
        }
    };

    template <typename Callable>
    Callable makeCallablePoolExecutable(Callable callable) {
        return CallablePoolExecutable<Callable>(move(callable));
    }
}

#endif //ANBASE_POOLEXECUTABLE_H
