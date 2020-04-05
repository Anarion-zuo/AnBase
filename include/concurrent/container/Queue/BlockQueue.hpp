//
// Created by anarion on 2020/2/2.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_BLOCK_QUEUE_HPP
#define MYCPPLIB_BLOCK_QUEUE_HPP

#include "container/Queue/ListQueue.hpp"
#include "concurrent/base/Mutex.h"
#include "concurrent/base/CondVar.h"
#include "container/base/container_utility.hpp"

namespace anarion {
    template <typename T> class BlockQueue {
    protected:

        LinkedList<T> list;
        Mutex lock;
        CondVar cond;

        constexpr bool __private_empty() const { return list.empty(); }

    public:
        BlockQueue() : cond(lock) {}
        BlockQueue(const BlockQueue<T> &rhs) : list(rhs.list), lock(), cond(lock) {}
        BlockQueue(BlockQueue<T> &&rhs) noexcept : list(forward<LinkedList<T>>(rhs.list)), lock(), cond(lock) {}

        BlockQueue<T> &operator=(const BlockQueue<T> &rhs) {
            if (&rhs == this) { return *this; }
            lock.lock();
            list = rhs.list;
            lock.unlock();
            return *this;
        }

        BlockQueue<T> &operator=(BlockQueue<T> &&rhs) noexcept {
            lock.lock();
            list = forward<LinkedList<T>>(rhs.list);
            lock.unlock();
            return *this;
        }

        bool empty() {
            lock.lock();
            bool flag = list.empty();
            lock.unlock();
            return flag;
        }

        void push(const T &o) {
            lock.lock();
            list.push_front(o);
            cond.signal();
            lock.unlock();
        }

        void push(T &&o) {
            lock.lock();
            list.push_front(forward<T>(o));
            cond.signal();
            lock.unlock();
        }

        T pop() {
            lock.lock();
            /*
             * it must not be the check empty function of this class
             * this class must make sure of correctness therefore add lock to each public functions
             * we do not need extra lock here
             */
//            cond.wait(bind(&LinkedList<T>::size, list));
            while (list.empty()) {
                cond.wait();
            }
            T o = list.pop_back();
            lock.unlock();
            return move(o);
        }
    };
}

#endif //MYCPPLIB_BLOCK_QUEUE_HPP
