//
// Created by anarion on 2020/3/5.
//

#ifndef MYCPPBASE_ROOTOBJECT_H
#define MYCPPBASE_ROOTOBJECT_H

namespace anarion {
    class RootObject {
    public:

        RootObject() = default;
        RootObject(const RootObject &) = default;
        RootObject(RootObject &&) = default;
        virtual ~RootObject() = default;
        RootObject &operator=(const RootObject &) = default;
        RootObject &operator=(RootObject &&) = default;

        template <typename T>
        constexpr T *interpretType() { return reinterpret_cast<T*>(this); }


    };
}

#endif //MYCPPBASE_ROOTOBJECT_H
