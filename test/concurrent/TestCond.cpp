//
// Created by anarion on 12/30/20.
//

#include <gtest/gtest.h>
#include <concurrent/base/Cond.h>
#include <concurrent/base/Thread.h>

using namespace anarion;

TEST(TestCond, TestCondInit) {
    Mutex mutex;
    Cond<Mutex> cond(mutex);
    Time sleepTime(1, 0);
    bool flag = false;
    Time startTime { Time::now() };
    auto thread { makeThread([&] {
        mutex.lock();
        while (!flag) {
            cond.waitRaw();
        }
        printf("slave time duration %lfs\n", Time(Time::difference(Time::now(), startTime)).getDouble());
        printf("hello cond\n");
    }) };
    thread.start();
    printf("wait for %lfs\n", sleepTime.getDouble());
    ThreadCore::sleep(sleepTime);
    flag = true;
    cond.signal();
    thread.join();
    Time joinTime { Time::now() };
    auto duration = Time::difference(joinTime, startTime);
    printf("master time duration %lfs\n", Time(duration).getDouble());
}
