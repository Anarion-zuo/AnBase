//
// Created by anarion on 5/12/20.
//

#ifndef ANBASE_POOL_H
#define ANBASE_POOL_H

#include <concurrent/base/CondVar.h>
#include <container/Stack/Stack.hpp>

namespace anarion {
    template<typename T>
    class Pool {
    protected:
        Mutex listLock;
        CondVar cond;
        Stack<T*> itemList;

        static T *allocateItem() { return new T; }
    public:
        Pool() : listLock(), cond(listLock) {}

        void returnItem(T *item) {
            listLock.lock();
            itemList.push(item);
            cond.signal();
            listLock.unlock();
        }

        T *fetchItem() {
            T *item;

            listLock.lock();
            if (itemList.empty()) {
                item = allocateItem();
            } else {
                item = itemList.pop();
            }
            listLock.unlock();

            return item;
        }
    };
}

#endif //ANBASE_POOL_H
