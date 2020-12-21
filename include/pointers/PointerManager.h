//
// Created by anarion on 5/12/20.
//

#ifndef ANBASE_POINTERMANAGER_H
#define ANBASE_POINTERMANAGER_H

#include <container/Map/HashMap.hpp>
#include <concurrent/base/Mutex.h>

namespace anarion {

    struct PointerException : std::exception {};

    struct PointerNotUnique : public PointerException {
        const char *what() const noexcept override {
            return "Initializing a unique pointer with a not unique pointer";
        }
    };

    struct PointerSupposedUnique : public PointerException {
        const char *what() const noexcept override {
            return "Initializing a shared pointer with a unique pointer";
        }
    };

    /*
     * Pointer Set providing service to both Unique and Shared Pointers
     * Singleton object as global service
     *
     * Accounting all pointers registered in all kinds of pointer classes
     *
     */

    class PointerManager {
    protected:

        Mutex mapLock;
        HashMap<void *, size_type> sharedMap;

        Mutex setLock;
        HashSet<void*> uniqueSet;

        static PointerManager *instance;

        PointerManager() = default;

    public:
        static PointerManager &get() { return *instance; }

        PointerManager(const PointerManager &) = delete;
        PointerManager(PointerManager &&) = delete;
        PointerManager &operator=(const PointerManager &) = delete;
        PointerManager &operator=(PointerManager &&) = delete;

        void registerUnique(void *p);
        void registerShared(void *p);
        void removeUnique(void *p);  // should follow a delete operation
        bool tryRemoveShared(void *p);  // should check whether to delete
        size_type sharedCount(void *p);
    };
}
#endif //ANBASE_POINTERMANAGER_H
