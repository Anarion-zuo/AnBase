//
// Created by anarion on 8/30/20.
//

#ifndef ANBASE_RUNTIMEDATA_H
#define ANBASE_RUNTIMEDATA_H

#include <io/fs/Path.h>

namespace anarion {
    class RunTimeData {
    protected:
        Path workPath;
        void updatePath();
    };
}

#endif //ANBASE_RUNTIMEDATA_H
