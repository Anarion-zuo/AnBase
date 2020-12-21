//
// Created by anarion on 12/15/20.
//

#ifndef ANBASE_LOCALPOINTER_H
#define ANBASE_LOCALPOINTER_H

namespace anarion {
    template <typename T>
    class LocalPointer {
    private:
        T *ptr = nullptr;
    public:
        T &operator*() { return *ptr; }
        const T &operator*() const { return *ptr; }
        T *operator->() { return ptr; }
        const T *operator->() const { return ptr; }

        LocalPointer() = default;
        LocalPointer(const LocalPointer<T> &) = delete;
        LocalPointer(LocalPointer &&rhs) noexcept
            : ptr(rhs.ptr) { rhs.ptr = nullptr; }

        void release() {
            delete ptr;
            ptr = nullptr;
        }
        ~LocalPointer() { release(); }

        template<typename Args...>
        LocalPointer(Args ...args) : ptr(new T(args...)) {}

        LocalPointer operator=(const LocalPointer &) = delete;
        LocalPointer operator=(LocalPointer &&rhs) noexcept {
            release();
            ptr = rhs.ptr;
            rhs.ptr = nullptr;
        }

        bool isNull() const { return ptr == nullptr; }
    };
}

#endif //ANBASE_LOCALPOINTER_H
