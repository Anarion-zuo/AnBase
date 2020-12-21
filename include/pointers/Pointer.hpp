//
// Created by anarion on 5/12/20.
//

#ifndef ANBASE_POINTER_HPP
#define ANBASE_POINTER_HPP

namespace anarion {
    template<typename T>
    class Pointer {
    protected:
        T *pointer = nullptr;
    public:
        Pointer() = default;
        explicit Pointer(T *p) : pointer(pointer) {}
        Pointer(Pointer &&rhs) noexcept : pointer(rhs.pointer) {
            rhs.pointer = nullptr;
        }

        void release() { delete pointer; pointer = nullptr; }

        constexpr T *operator->() { return pointer; }
        constexpr T &operator*() { return *pointer; }
        constexpr T *getRawPointer() { return pointer; }
        constexpr T &getReference() { return *pointer; }
    };
}

#endif //ANBASE_POINTER_HPP
