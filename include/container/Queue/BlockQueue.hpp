//
// Created by anarion on 2020/2/2.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_BLOCK_QUEUE_HPP
#define MYCPPLIB_BLOCK_QUEUE_HPP

#include "ListQueue.hpp"
#include "../../concurrent/base/Mutex.h"
#include "../../concurrent/base/CondVar.h"
#include "../../container/base/container_utility.hpp"

namespace anarion {
    template <typename T> class BlockQueue {
    protected:
        struct CondFunc {
            CondFunc() = default;
            explicit CondFunc(BlockQueue<T> *_this) : _this(_this) {}
            BlockQueue<T> *_this = nullptr;
            bool operator ()() {
                return !_this->empty();
            }
        };

        LinkedList<T> list;
        CondVar<CondFunc> cond;

    public:
        BlockQueue() { cond.getFuncObject()._this = this; }
        BlockQueue(const BlockQueue<T> &rhs) : list(rhs.list) { cond.getFuncObject()._this = this; }
        BlockQueue(BlockQueue<T> &&rhs) noexcept : list(forward<LinkedList < T>>(rhs.list)) { cond.getFuncObject()._this = this; }

        BlockQueue<T> &operator=(const BlockQueue<T> &rhs) {
            if (&rhs == this) { return *this; }
            list = rhs.list;
            cond.getFuncObject()._this = this;
            return *this;
        }

        BlockQueue<T> &operator=(BlockQueue<T> &&rhs) noexcept {
            list = forward<LinkedList<T>>(rhs.list);
            cond.getFuncObject()._this = this;
            return *this;
        }

        bool empty() const { return list.empty(); }

        void put(const T &o) {
            cond.lock();
            list.push_back(o);
            cond.unlock();
            cond.signalAll();
        }

        void put(T &&o) {
            cond.lock();
            list.push_back(forward<T>(o));
            cond.unlock();
            cond.signalAll();
        }

        T pop() {
            cond.wait();
            T o = list.pop_back();
            cond.unlock();
            return move(o);
        }
    };
}

#endif //MYCPPLIB_BLOCK_QUEUE_HPP
