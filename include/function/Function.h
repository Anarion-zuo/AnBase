//
// Created by anarion on 12/15/20.
//

#ifndef ANBASE_FUNCTION_H
#define ANBASE_FUNCTION_H

#include <container/base/container_utility.hpp>

namespace anarion {

    template <size_type index, typename ...Args>
    struct ArgsIndexedType;
    template <size_type index, typename First, typename ...Args>
    struct ArgsIndexedType<index, First, Args...> {
        using type = typename ArgsIndexedType<index - 1, Args...>::type;
    };
    template <typename First, typename ...Args>
    struct ArgsIndexedType<0, First, Args...> {
        using type = First;
    };

    template <typename Signature>
    struct FunctionTraits;

    template <typename ReturnType, typename ...Args>
    struct FunctionTraits<ReturnType(*)(Args...)> {
        using RetType = ReturnType;
    };

    // only a declaration
    template <typename Signature>
    class Function;

    template <typename RetType, typename ...Args>
    class Function<RetType(Args...)> {
    private:
        using func_pt = RetType(*)(Args...);
        func_pt fp;
    public:
        explicit Function(func_pt fp) : fp(fp) {}
        Function(const Function &rhs) : fp(rhs.fp) {}
        RetType operator()(Args ...args) {
            return fp(forward<Args>(args)...);
        }
    };

    template <>
    class Function<void()> {
    private:
        using func_pt = void(*)();
        func_pt fp;
    public:
        explicit Function(func_pt fp) : fp(fp) {}
        Function(const Function &rhs) : fp(rhs.fp) {}
        void operator()() { fp(); }
    };

    template <typename RetType>
    class Function<RetType()> {
    private:
        using func_pt = RetType(*)();
        func_pt fp;
    public:
        explicit Function(func_pt fp) : fp(fp) {}
        Function(const Function &rhs) : fp(rhs.fp) {}
        void operator()() { fp(); }
    };

    template <typename ...Args>
    class Function<void(Args...)> {
    private:
        using func_pt = void(*)(Args...);
        func_pt fp;
    public:
        explicit Function(func_pt fp) : fp(fp) {}
        Function(const Function &rhs) : fp(rhs.fp) {}

        void operator()(Args ...args) {
            fp(forward<Args>(args)...);
        }
    };

    template <typename Callable, typename RetType, typename ...Args>
    class WrappedCallable {
    private:
        Callable callable;
        RetType(Callable::*callableOperatorBracket)(Args...) const ;

    public:
        explicit WrappedCallable(const Callable &callable) : callable(callable) {}
        WrappedCallable(WrappedCallable &&rhs) noexcept : callable(move(rhs.callable)) {}
        RetType operator() (Args ...args) const {
            return callable.operator()(forward<Args>(args)...);
        }
    };

    template <typename Callable, typename ...Args>
    class WrappedCallable<Callable, void, Args...> {
    private:
        Callable callable;

    public:
        explicit WrappedCallable(const Callable &callable) : callable(callable) {}
        WrappedCallable(WrappedCallable &&rhs) noexcept : callable(move(rhs.callable)) {}
        void operator() (Args ...args) const {
            callable(forward<Args>(args)...);
        }
    };

}

#endif //ANBASE_FUNCTION_H
