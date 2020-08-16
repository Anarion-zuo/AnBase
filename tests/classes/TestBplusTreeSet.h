//
// Created by anarion on 8/11/20.
//

#ifndef ANBASE_TESTBPLUSTREESET_H
#define ANBASE_TESTBPLUSTREESET_H

#include <test/TestClass.h>
#include <container/Set/BplusTreeSet.h>

namespace anarion {
    class TestBplusTreeSet : public TestClass {
    protected:
        void runner() override {
            BplusTreeSet<int, 2> treeSet;

            int insertSize = 100;

            // test insert
            for (size_type index = 0; index < insertSize; ++index) {
                printf("Tree Begins\n");
                treeSet.insert(index);
                if (!treeSet.testChildParentPointers()) {
                    throwTestFailed();
                }
                treeSet.print();
            }
            // test find
            for (size_type index = 0; index < insertSize; ++index) {
                if (!treeSet.has(index)) {
                    throwTestFailed();
                }
            }

            // test iterator
            for (auto it = treeSet.begin_iterator(); it != treeSet.end_iterator(); ++it) {
                printf("%d ", *it);
            }
            printf("\n");

            // test iterator increment operation
            size_type stepCount = 0;
            auto incrementIt = treeSet.begin_iterator();
            for (; stepCount < insertSize; incrementIt += 3, stepCount += 3) {
                if (*incrementIt != stepCount) {
                    throwTestFailed();
                }
            }
            incrementIt = treeSet.end_iterator();
            --incrementIt;
            stepCount = *incrementIt;
            // test iterator decrement operation
            for (; stepCount < insertSize;
            incrementIt -= 3u,
            stepCount -= 3) {
                if (*incrementIt != stepCount) {
                    throwTestFailed();
                }
            }
            return;
        }
    };
}

#endif //ANBASE_TESTBPLUSTREESET_H
