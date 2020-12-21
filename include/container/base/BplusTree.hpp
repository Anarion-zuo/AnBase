//
// Created by 左钰 on 2020/5/24.
//

#ifndef ANBASE_BPLUSTREE_HPP
#define ANBASE_BPLUSTREE_HPP

#include "container_utility.hpp"
#include "../List/Vector.hpp"

namespace anarion {
template <typename T, typename K, typename getKeyCall>
class BplusTree {
private:
    static const K &getKey(const T &obj) {
        return getKeyCall(obj);
    }

    struct Node {
        Vector<K> keys;
        Vector<Node *> childs;

        const K &getKey(size_type index) const {
            return keys.get(index);
        }
        Node *getLeftChildOf(size_type keyIndex) const {

        }
        Node *getRightChildOf(size_type keyIndex) const {

        }
    };

public:

};
}

#endif //ANBASE_BPLUSTREE_HPP
