//
// Created by anarion on 2020/3/11.
//

#ifndef MYCPPBASE_POINTER_H
#define MYCPPBASE_POINTER_H

namespace anarion {
    template<typename T>
    class Pointer {
    protected:
        T *ptr;
    public:
        Pointer(T *ptr) : ptr(ptr) {}
        Pointer(Pointer<T> &&rhs) noexcept : ptr(rhs.ptr) { rhs.ptr = nullptr; }
        ~Pointer() { delete ptr; }
        Pointer<T> &operator=(Pointer<T> &&rhs) noexcept { delete ptr; ptr = rhs.ptr; rhs.ptr = nullptr; return *this; }

        T &operator*() { return *ptr; }
        T *operator->() const { return ptr; }

        void release() { delete ptr; ptr = nullptr; }
    };
}

#endif //MYCPPBASE_POINTER_H
