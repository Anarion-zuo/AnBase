//
// Created by 左钰 on 2020/5/24.
//

#ifndef ANBASE_EXTHASHSET_HPP
#define ANBASE_EXTHASHSET_HPP

#include "HashSet.hpp"

namespace anarion {
    template <typename T, size_type slotSize = 8, typename hash_func = hash_function<T>>
    class ExtHashSet {
    protected:

        using bitcount_type = unsigned char;

        struct hash_node {
            hash_node *next = nullptr;
            hash_type hashVal;
            T obj;
            explicit hash_node(const T &obj, hash_type hash_val) : obj(obj), hashVal(hash_val) {}
            explicit hash_node(T &&obj, hash_type hash_val) : obj(forward<T>(obj)), hashVal(hash_val) {}
        };

        struct hash_head {
            hash_node *next = nullptr;
            bitcount_type localCount;
            size_type itemCount = 0;
            explicit hash_head(bitcount_type count) : localCount(count) {}
        };

        hash_head **heads;
        bitcount_type globalCount;

        constexpr size_type getHeadsCount() const {
            return 1ul << globalCount;
        }

        void initMembers() {
            globalCount = 1;
            size_type headsCount = getHeadsCount();
            // heads
            heads = static_cast<size_type *>(operator new(sizeof(hash_head *) * headsCount));
            for (size_type index = 0; index < headsCount; ++index) {
                heads[index] = new hash_head(globalCount);
            }
        }

        constexpr size_type getHeadIndexByGlobalCount(hash_type hashVal) const {
            size_type offset = sizeof(size_type) - globalCount;
            hash_type modder = -1ul;
            modder <<= globalCount;
            return hashVal & (~modder);
        }

        void extentHeads() {
            size_type oldHeadsCount = getHeadsCount();
            ++globalCount;
            size_type newHeadsCount = getHeadsCount();
            hash_node **newHeads = (hash_node*)operator new (sizeof(hash_node*) * newHeadsCount);
            memcpy(newHeads, heads, sizeof(hash_node*) * oldHeadsCount);
            // set up new heads
            for (size_type index = oldHeadsCount; index < newHeadsCount; ++index) {

            }
        }

        void insertNode(hash_node *node) {
            // get everything
            size_type headIndex = getHeadIndexByGlobalCount(node->hashVal);
            size_type &headCount = heads[headIndex]->itemCount;
            hash_node * &head = heads[headIndex];
            // empty slot
            if (head == nullptr) {
                head = node;
                headCount = 1;
                return;
            }
            if (headCount < slotSize) {
                hash_node *oldHead = head;
                head = node;
                head->next = oldHead;
                ++headCount;
                return;
            }
            // must split
            extentHeads();

        }

    public:
        ExtHashSet() {
            initMembers();
        }
        ExtHashSet(ExtHashSet &&rhs) : heads(rhs.heads), globalCount(rhs.globalCount) {
            rhs.initMembers();
        }

        void insert(const T &obj) {
            hash_func func;
            hash_type hashVal = func(obj);
            hash_node *node = new hash_node(obj, hashVal);
            insertNode(node);
        }

    };
}

#endif //ANBASE_EXTHASHSET_HPP
