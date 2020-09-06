//
// Created by anarion on 9/6/20.
//

#ifndef ANBASE_ROUTINE_H
#define ANBASE_ROUTINE_H

/*
 * Routines
 *
 * A routine is a function call with the form of func().
 * A class may override operator() to provide such general interface.
 * Therefore, a routine is a callable object with valid operator(...).
 *
 * Here is some general interface calling a routine.
 */

#include <container/base/container_utility.hpp>
#include <container/Bind/Tuple.hpp>

/*
 * Bind Routine
 * This class is for a late call. The parameters are stored inside the class.
 *      The designed call can be called from any time any place with the same parameters.
 *      The return value should be stored also.
 *
 * There are some boundary conditions worth noting.
 *  - The callee may or may not have a return value.
 *  - The callee may or may not have passing parameters.
 * The specifications above must be tested.
 *
 *
 */

namespace anarion {
    template <typename CallableType, typename RetType, typename ...Args>
    class BindRoutine {
    protected:
        Tuple<Args...> args;
        RetType ret;
        CallableType callable;
    public:
        explicit BindRoutine(CallableType callable, Args... args)
                : callable(callable), args(forward<Args>(args)...) {}

        void run() {
            ret = tupleArgsCall(callable, args);
        }
        void operator()() {
            run();
        }
        RetType &getReturn() {
            return ret;
        }
        const RetType &getReturn() const {
            return ret;
        }
    };

    template <typename RetType, typename CallableType, typename ...Args>
    BindRoutine<CallableType, RetType, Args...> makeBindRoutine(CallableType callable, Args ...args) {
        return BindRoutine<CallableType, RetType, Args...>(callable, args...);
    }

    template <typename CallableType, typename ...Args>
    class BindRoutine<CallableType, void, Args...> {
    protected:
        Tuple<Args...> args;
        CallableType callable;
    public:
        explicit BindRoutine(CallableType callable, Args... args)
                : callable(callable), args(forward<Args>(args)...) {}

        void run() {
            tupleArgsCallNoReturn(callable, args);
        }

        void operator()() {
            run();
        }
    };

    template <typename CallableType, typename ...Args>
    BindRoutine<CallableType, void, Args...> makeBindRoutine(CallableType callable, Args ...args) {
        return BindRoutine<CallableType, void, Args...>(callable, args...);
    }
}

#endif //ANBASE_ROUTINE_H
