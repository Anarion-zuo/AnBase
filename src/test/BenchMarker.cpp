//
// Created by anarion on 8/10/20.
//

#include "BenchMarker.h"

void anarion::BenchMarker::runBenchMark() {
    printf("Begins benchmark %s...\n", testName());
    beginTime.setCurrent();
    testee();
    endTime.setCurrent();
    diffTime = Time(Time::difference(endTime, beginTime));
    printf("Ends benchmark %s...\nDuration Time: %lf\n", testName(), diffTime.getDouble());
}
