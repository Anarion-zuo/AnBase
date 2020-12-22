//
// Created by anarion on 12/22/20.
//

#include <concurrent/pool/ArrayThreadPool.h>
#include <gtest/gtest.h>
#include <function/Function.h>

using namespace anarion;

TEST(TestArrayThreadPool, TestInit) {
    size_type length = 10;
    ArrayThreadPool pool(length);
    pool.start(0);
    auto fp = []() {
        printf("hello thread pool\n");
    };
    PoolExecutable *executable = new CallablePoolExecutable<decltype(fp)>(fp);
    pool.setTask(0, executable);
    pool.launch(0);
    ThreadCore::sleep(Time{1000});
    pool.join(0);
}
