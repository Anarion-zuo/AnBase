//
// Created by anarion on 8/16/20.
//

#ifndef ANBASE_TESTBPLUSTREEMAP_H
#define ANBASE_TESTBPLUSTREEMAP_H

#include <test/TestClass.h>
#include <container/Map/BplusTreeMap.hpp>
#include <map>

namespace anarion {
class TestBplusTreeMap : public TestClass {
protected:
    void runner() override {
        BplusTreeMap<int, unsigned int, 100> map;
        size_type insertSize = 1000;
        std::map<int, unsigned int> stdMap;
        Time begin, end;
        begin.setClockType(CLOCK_THREAD_CPUTIME_ID);
        end.setClockType(CLOCK_THREAD_CPUTIME_ID);
        double mydiff, stdDiff;

        begin.setCurrent();
        for (size_type index = 0; index < insertSize; ++index) {
            map.insert({index, index + 1});
        }
        end.setCurrent();
        mydiff = Time(Time::difference(end, begin)).getDouble();

        begin.setCurrent();
        for (size_type index = 0; index < insertSize; ++index) {
            stdMap.insert({index, index + 1});
        }
        end.setCurrent();
        stdDiff = Time(Time::difference(end, begin)).getDouble();

        printf("inserting: my b+ tree %lf, std map %lf\n", mydiff, stdDiff);

        begin.setCurrent();
        for (size_type index = 0; index < insertSize; ++index) {
            auto it = map.find(index);
            if (it->get_val() != index + 1) {
                throwTestFailed();
            }
        }
        end.setCurrent();
        mydiff = Time(Time::difference(end, begin)).getDouble();
        begin.setCurrent();
        for (size_type index = 0; index < insertSize; ++index) {
            auto it = stdMap.find(index);
            if (it->second != index + 1) {
                throwTestFailed();
            }
        }
        end.setCurrent();
        stdDiff = Time(Time::difference(end, begin)).getDouble();
        printf("finding: my b+ tree %lf, std map %lf\n", mydiff, stdDiff);
    }
};
}

#endif //ANBASE_TESTBPLUSTREEMAP_H
