//
// Created by 左钰 on 2020/5/24.
//

#ifndef ANBASE_BPLUSTREE_HPP
#define ANBASE_BPLUSTREE_HPP

#include "container_utility.hpp"

namespace anarion {
    template <typename T>
    class BplusTree {
    protected:
        static constexpr bool isLessThan(const T &o1, const T &o2) { return o1 < o2; }
        static constexpr bool isGeThan(const T &o1, const T &o2) { return !isLess(o1, o2); }

        const size_type segSize = 2;

        struct Node {
            Vector<T> array;
            Vector<Node*> childs;
            Node *parent = nullptr;
            size_type parentIndex = 0;
        };

        struct Leaf : public Node {
            Leaf *prev = nullptr, *next = nullptr;
        };

        Node *root = new Leaf;

        void insertWithoutSplit() {

        }

        static size_type searchBinaryNode(Node *node, const T &obj) {
            size_type left = 0, right = node->array.size();
            // binary search
            bool isGreater = false;
            while (right != left) {
                size_type midIndex = ((right - left) >> 1ul) + left;
                T &midObj = node->array[midIndex];
                if (obj < midObj) {
                    right = midIndex;
                    isGreater = false;
                } else if (obj > midObj) {
                    isGreater = true;
                    left = midIndex + 1;
                } else {
                    left = midIndex;  // left as result
                    break;
                }
            }
            return left;
        }

    public:
        BplusTree() {}
        explicit BplusTree(size_type segSize) : segSize(segSize) {}

        void insert(const T &obj) {
            if (root->array.empty()) {
                root->array.push_back(obj);
                root->childs.push_back(nullptr);
                root->childs.push_back(nullptr);
                return;
            }
            Node *node = root;
            size_type lastIndex;
            while (true) {
                lastIndex = searchBinaryNode(node, obj);
                // left as result
                Node *next;
                next = node->childs[lastIndex];
                if (next == nullptr) {
                    break;
                }
                node = next;
            }
            node->array.insert(lastIndex, obj);
            node->childs.insert(lastIndex, nullptr);

            // split
            while (true) {
                if (node->array.size() <= segSize) {
                    // no split
                    break;
                }
                // must split
                Node *parent = node->parent;
                if (parent == nullptr) {
                    // is root
                    parent = new Node;
                    parent->childs.push_back(node);
                    node->parent = parent;
                    root = parent;
                }
                size_type halfSize = segSize / 2;
                T &midObj = node->array[halfSize];
                /*
                 * Last index is not the index of
                 */
                size_type parentIndex = node->parentIndex;
                parent->array.insert(parentIndex, midObj);  // move to parent
                // move the right half
                Node *newRight = new Node;
                newRight->array.insert(0, &node->array[halfSize], node->array.size() - halfSize);
                node->array.resize(halfSize);
                // parent
                newRight->parent = parent;
                newRight->parentIndex = parentIndex + 1;
                // childs
                parent->childs.insert(parentIndex + 1, newRight);
                newRight->childs.insert(0, &node->childs[halfSize], node->childs.size() - halfSize + 1);
                node->childs.resize(halfSize);
                node->childs.push_back(nullptr);
                // update
                node = parent;
            }

        }
    };
}

#endif //ANBASE_BPLUSTREE_HPP
