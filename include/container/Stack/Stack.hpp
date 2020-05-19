//
// Created by anarion on 2020/2/21.
//

#ifndef MYCPPBASE_STACK_HPP
#define MYCPPBASE_STACK_HPP

#include "../List/LinkedList.hpp"

namespace anarion {
    template<typename T>
    class Stack {
    protected:
        LinkedList<T> list;

    public:
        Stack() = default;
        Stack(const Stack &) = default;
        Stack(Stack &&) noexcept = default;
        ~Stack() = default;
        Stack &operator=(const Stack &) = default;
        Stack &operator=(Stack &&) noexcept = default;

        constexpr bool empty() const { return list.empty(); }
        constexpr void push(const T &o) { list.push_back(o); }
        constexpr void push(T &&o) { list.push_back(forward<T>(o)); }
        constexpr T pop() { return list.pop_back(); }
    };
}

#endif //MYCPPBASE_STACK_HPP
