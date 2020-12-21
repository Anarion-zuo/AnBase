//
// Created by anarion on 8/15/20.
//

#ifndef ANBASE_BPLUSTREEMAP_HPP
#define ANBASE_BPLUSTREEMAP_HPP

#include "../Set/BplusTreeSet.h"
#include "MapEntry.hpp"

namespace anarion {
    template <typename T, typename K, size_type levelSize = 4>
    class BplusTreeEmbededMap : public BplusTreeSet<T, levelSize, K, EntryGetKeyCaller<T, K>> {};

    template <typename K, typename V, size_type levelSize = 4>
    class BplusTreeMap : public BplusTreeEmbededMap<MapEntry<K, V>, K, levelSize> {};
}

#endif //ANBASE_BPLUSTREEMAP_HPP
