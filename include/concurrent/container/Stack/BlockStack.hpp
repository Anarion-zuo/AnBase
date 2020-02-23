//
// Created by anarion on 2020/2/21.
//

#ifndef MYCPPBASE_BLOCKSTACK_H
#define MYCPPBASE_BLOCKSTACK_H

#include <concurrent/base/CondVar.h>
#include "container/List/LinkedList.hpp"

namespace anarion {

    template <typename T>
    class BlockStack {
    protected:
        LinkedList<T> list;
        Mutex lock;
        CondVar cond;
    public:
        BlockStack() : lock(), cond(lock) {}
        BlockStack(const BlockStack &rhs) : list(rhs.list), lock(), cond(lock) {}
        BlockStack(BlockStack &&rhs) noexcept : list(forward<LinkedList>(rhs.list)), lock(), cond(lock) {}

        BlockStack &operator=(const BlockStack &rhs) {
            if (&rhs == this) { return *this; }
            lock.lock();
            list = rhs.list;
            lock.unlock();
            return *this;
        }

        BlockStack &operator=(BlockStack &rhs) noexcept {
            lock.lock();
            list = forward<LinkedList>(rhs.list);
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
            list.push_back(o);
            cond.signal();
            lock.unlock();
        }

        void push(T &&o) {
            lock.lock();
            list.push_back(forward<T>(o));
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
            cond.wait(bind(&LinkedList<T>::size, list));
            T o = list.pop_back();
            lock.unlock();
            return move(o);
        }
    };
}

#endif //MYCPPBASE_BLOCKSTACK_H
