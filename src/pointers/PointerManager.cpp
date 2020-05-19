//
// Created by anarion on 5/12/20.
//

#include "pointers/PointerManager.h"

anarion::PointerManager *anarion::PointerManager::instance = new PointerManager;

void anarion::PointerManager::registerUnique(void *p) {
    // check whether in map
    mapLock.lock();
    auto mapIt = sharedMap.find(p);
    if (mapIt != sharedMap.end_iterator()) {
        mapLock.unlock();
        throw PointerNotUnique();
    }
    mapLock.unlock();

    // check whether in set
    setLock.lock();
    auto setIt = uniqueSet.find(p);
    if (setIt != uniqueSet.end_iterator()) {
        setLock.unlock();
        throw PointerNotUnique();
    }
    uniqueSet.insert(p);
    setLock.unlock();
}

void anarion::PointerManager::registerShared(void *p) {
    setLock.lock();
    auto setIt = uniqueSet.find(p);
    if (setIt != uniqueSet.end_iterator()) {
        setLock.unlock();
        throw PointerSupposedUnique();
    }
    setLock.unlock();

    mapLock.lock();
    auto mapIt = sharedMap.find(p);
    if (mapIt == sharedMap.end_iterator()) {
        sharedMap.insert({p, 1});
    } else {
        ++mapIt->get_val();
    }
    mapLock.unlock();
}

void anarion::PointerManager::removeUnique(void *p) {
    setLock.lock();
    uniqueSet.remove(p);
    setLock.unlock();
}

bool anarion::PointerManager::tryRemoveShared(void *p) {
    bool isRemovable = false;

    mapLock.lock();

    auto it = sharedMap.find(p);
    size_type &count = it->get_val();
    if (count > 1) {
        --count;
    } else {
        isRemovable = true;
        sharedMap.remove(it);
    }

    mapLock.unlock();

    return isRemovable;
}

anarion::size_type anarion::PointerManager::sharedCount(void *p) {
    size_type ret = 0;
    mapLock.lock();

    auto it = sharedMap.find(p);
    if (it != sharedMap.end_iterator()) {
        ret = it->get_val();
    }

    mapLock.unlock();
    return ret;
}
