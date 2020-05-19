//
// Created by anarion on 5/12/20.
//

#ifndef ANBASE_SHAREDPOINTER_HPP
#define ANBASE_SHAREDPOINTER_HPP

#include "Pointer.hpp"

namespace anarion {
    template<typename T>
    class SharedPointer : public Pointer<T> {
    protected:
        void deletePointer() {
            if (this->pointer == nullptr) { return; }
            bool isRemovable = PointerManager::get().tryRemoveShared(this->pointer);
            if (isRemovable) {
                this->release();
            }
            this->pointer = nullptr;
        }

    public:
        SharedPointer() = default;
        explicit SharedPointer(T *pointer) : Pointer<T>(pointer) {
            PointerManager::get().registerShared(pointer);
        }
        SharedPointer(SharedPointer &&rhs) noexcept : Pointer<T>(forward<SharedPointer<T>>(rhs)) {}
        SharedPointer(const SharedPointer &rhs) : Pointer<T>(rhs.pointer) {
            PointerManager::get().registerShared(this->pointer);
        }
        SharedPointer &operator=(const SharedPointer<T> &rhs) {
            if (&rhs == this) { return *this; }
            deletePointer();
            this->pointer = rhs.pointer;
            PointerManager::get().registerShared(this->pointer);
            return *this;
        }
        SharedPointer &operator=(SharedPointer<T> &&rhs) noexcept {
            deletePointer();
            this->pointer = rhs.pointer;
            rhs.pointer = nullptr;
            return *this;
        }
        ~SharedPointer() { deletePointer(); }

        size_type sharedCount() const {
            return PointerManager::get().sharedCount(this->pointer);
        }
    };
}

#endif //ANBASE_SHAREDPOINTER_HPP
