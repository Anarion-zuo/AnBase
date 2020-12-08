//
// Created by anarion on 12/8/20.
//

#ifndef ANBASE_SOFTARRAY_H
#define ANBASE_SOFTARRAY_H

#include "Vector.hpp"

namespace anarion {
template <typename T>
class SoftArray {
private:
    Vector<T> realArray;
public:
    SoftArray() : realArray(8) {}
    SoftArray(const SoftArray &) = default;
    SoftArray(SoftArray &&) noexcept = default;
    ~SoftArray() = default;

    void set(size_type index, const T &obj) {
        if (index < realArray.size()) {
            T &target = realArray.get(index);
            Copier<T>().clearSpace(&target, 1);
            new (&target) T(obj);
        } else if (index < realArray.capacity()) {
            size_type more = index - realArray.size();
            for (size_type i = 0; i < more; ++i) {
                realArray.pushBack(T());
            }
            realArray.pushBack(obj);
        } else {
            realArray.resize((index + 1) * 2);
            size_type more = index - realArray.size();
            for (size_type i = 0; i < more; ++i) {
                realArray.pushBack(T());
            }
            realArray.pushBack(obj);
        }
    }

    void set(size_type index, T &&obj) {
        if (index < realArray.size()) {
            T &target = realArray.get(index);
            Copier<T>().clearSpace(&target, 1);
            new (&target) T(forward<T>(obj));
        } else if (index < realArray.capacity()) {
            size_type more = index - realArray.size();
            for (size_type i = 0; i < more; ++i) {
                realArray.pushBack(T());
            }
            realArray.pushBack(forward<T>(obj));
        } else {
            realArray.resize((index + 1) * 2);
            size_type more = index - realArray.size();
            for (size_type i = 0; i < more; ++i) {
                realArray.pushBack(T());
            }
            realArray.pushBack(forward<T>(obj));
        }
    }

    T &get(size_type index) {
        return realArray.get(index);
    }
    const T &get(size_type index) const {
        return realArray.get(index);
    }
};

    template <typename T>
    class SoftArray<T *> {
    private:
        Vector<T*> realArray;
    public:
        SoftArray() : realArray(8) {}
        SoftArray(const SoftArray &) = default;
        SoftArray(SoftArray &&) noexcept = default;
        ~SoftArray() = default;

        constexpr size_type size() const { return realArray.size(); }

        void set(size_type index, T *obj) {
            if (index < realArray.size()) {
                realArray.get(index) = obj;
            } else if (index < realArray.capacity()) {
                size_type more = index - realArray.size();
                for (size_type i = 0; i < more; ++i) {
                    realArray.pushBack(nullptr);
                }
                realArray.pushBack(obj);
            } else {
                realArray.resize((index + 1) * 2);
                size_type more = index - realArray.size();
                for (size_type i = 0; i < more; ++i) {
                    realArray.pushBack(nullptr);
                }
                realArray.pushBack(obj);
            }
        }

        T *get(size_type index) {
            return realArray.get(index);
        }
        const T *get(size_type index) const {
            return realArray.get(index);
        }
    };
}

#endif //ANBASE_SOFTARRAY_H
