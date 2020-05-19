//
// Created by anarion on 5/12/20.
//

#ifndef ANBASE_UNIQUEPOINTER_H
#define ANBASE_UNIQUEPOINTER_H

#include "PointerManager.h"
#include "Pointer.hpp"

namespace anarion {


    /*
     * Unique Pointer for convenient memory management
     * Should suffice under most circumstances, or use Shared Pointer instead
     *
     * Initialize a unique pointer with new operator, passing an object corresponding to type T
     * Dont simply pass an arbitrary pointer, or the class malfunctions when trying to delete the pointer
     *
     * The data of the class can only be passed via move constructor/assignment
     * Automatically delete the pointer when destructing
     * The object is passed on via move constructor/assignment
     */

    template<typename T>
    class UniquePointer : public Pointer<T> {
    protected:
        void deletePointer() {
            if (this->pointer == nullptr) { return; }
            this->release();
            PointerManager::get().removeUnique(this->pointer);
        }

    public:
        UniquePointer() = default;
        explicit UniquePointer(T *pointer) : Pointer<T>(pointer) {
            PointerManager::get().registerUnique(pointer);
        }
        UniquePointer(UniquePointer &&rhs) noexcept : Pointer<T>(forward<UniquePointer<T>&&>(rhs)) {}
        UniquePointer &operator=(UniquePointer<T> &&rhs) noexcept {
            deletePointer();
            this->pointer = rhs.pointer;
            rhs.pointer = nullptr;
            return *this;
        }
        UniquePointer &operator=(T *pointer) {
            PointerManager::get().registerUnique(pointer);
            deletePointer();
            this->pointer = pointer;
            return *this;
        }
        ~UniquePointer() {
            deletePointer();
        }

    };
}

#endif //ANBASE_UNIQUEPOINTER_H
