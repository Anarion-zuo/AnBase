//
// Created by anarion on 4/22/20.
//

#ifndef MYCPPBASE_SHAREDPOINTER_HPP
#define MYCPPBASE_SHAREDPOINTER_HPP

namespace anarion {
    struct ReferenceToNullPointer : std::exception {};

    template <typename T>
    class SharedPointer {
    protected:
        size_type *count = nullptr;
        T *ptr = nullptr;

        void release() {
            delete count;
            delete ptr;
            count = nullptr;
            ptr = nullptr;
        }
    public:
        SharedPointer() {}  // don't call this

        explicit SharedPointer(T *ptr) : ptr(ptr), count(new size_type(1)) {}

        SharedPointer(const SharedPointer<T> &rhs) : ptr(rhs.ptr), count(rhs.count) {
            if (ptr == nullptr || count == nullptr) {
                ptr = nullptr;
                count = nullptr;
                return ;
            }
            ++*count;
        }

        SharedPointer(SharedPointer<T> &&rhs) noexcept : ptr(rhs.ptr), count(rhs.count) {
            rhs.ptr = nullptr;
            rhs.count = nullptr;
        }

        ~SharedPointer() {
            if (*count == 1) {
                release();
                return;
            }
            --*count;
        }

        constexpr T &operator*() {
            if (count == nullptr || ptr == nullptr) {
                throw ReferenceToNullPointer();
            }
            return *ptr;
        }

        constexpr T *operator->() {
            if (count == nullptr) {
                return nullptr;
            }
            return ptr;
        }

        constexpr bool operator==(const SharedPointer &rhs) const {
            return ptr == rhs.ptr;
        }

        constexpr bool operator!=(const SharedPointer &rhs) const {
            return !(rhs == *this);
        }

        constexpr bool operator==(T *ptr) { return this->ptr == ptr; }
        constexpr bool operator!=(T *ptr) { return !operator==(ptr); }

        constexpr bool operator==(nullptr_t ptr) {
            return this->ptr == nullptr || count == nullptr;
        }

    };

}

#endif //MYCPPBASE_SHAREDPOINTER_HPP
