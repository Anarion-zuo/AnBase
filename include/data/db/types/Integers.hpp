//
// Created by anarion on 1/22/21.
//

#ifndef ANBASE_INTEGERS_HPP
#define ANBASE_INTEGERS_HPP

#include <cstring>
#include "DbTypes.h"

namespace anarion {
    namespace db {

        template <typename NumType>
        class IntegerCommon {
            NumType number;
        public:
            void writeBuffer(char *buf) const {
                memcpy(buf, &number, sizeof(NumType));
            }
            void loadBuffer(const char *buf) {
                memcpy(&number, buf, sizeof(NumType));
            }
            void set(NumType num) {
                number = num;
            }
            NumType get() const { return number; }
        };

        class Int8 : public IntegerCommon<int8_t> {};
        class Int16 : public IntegerCommon<int16_t> {};
        class Int32 : public IntegerCommon<int32_t> {};
        class Int64 : public IntegerCommon<int64_t> {};
    }
}

#endif //ANBASE_INTEGERS_HPP
