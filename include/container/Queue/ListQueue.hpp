//
// Created by anarion on 2020/2/2.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_LIST_QUEUE_HPP
#define MYCPPLIB_LIST_QUEUE_HPP

#include "../List/LinkedList.hpp"

namespace anarion {
    template <typename T> class ListQueue {
    protected:
        LinkedList<T> list;

    public:
        ListQueue() = default;
        ListQueue(const ListQueue<T> &rhs) : list(rhs.list) {}
        ListQueue(ListQueue<T> &&rhs) noexcept : list(move(rhs.list)) {}

        ListQueue<T> &operator=(const ListQueue<T> &rhs) {
            if (this == &rhs) { return *this; }
            list = rhs.list;
            return *this;
        }

        ListQueue<T> &operator=(ListQueue<T> &&rhs) noexcept {
            list = move(rhs.list);
            return *this;
        }

        void push(const T &o) { list.push_back(o); }
        void push(T &&o) { list.push_back(forward(o)); }
        T pop() { return list.pop_front(); }

        bool empty() const { return list.empty(); }
    };
};

#endif //MYCPPLIB_LIST_QUEUE_HPP
