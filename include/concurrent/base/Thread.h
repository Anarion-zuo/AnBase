//
// Created by 左钰 on 2020/1/15.
//

#ifndef MYCPPLIB_THREAD_H
#define MYCPPLIB_THREAD_H


#include <pthread.h>
#include "../../context/Callable.h"
#include "../../feature/UnCopyable.h"

namespace anarion {
    class Thread : virtual public Callable, virtual public UnCopyable {
        friend class ThreadPool;

    protected:
        pthread_t pid;

        static void *start_routine(void *p);

    public:

        Thread();
        Thread(Thread &&rhs) noexcept;

        Thread &operator=(Thread &&rhs) noexcept;

        void start();
        void join();   // dont join yourself

    };
}

#endif //MYCPPLIB_THREAD_H
