//
// Created by 左钰 on 2020/5/25.
//

#ifndef ANBASE_BTREE_HPP
#define ANBASE_BTREE_HPP

namespace anarion {
    template<typename T>
    class BTree {
    protected:
        const size_type degree = 2;

        struct Node {
            Vector<T> elements;
            Vector<Node*> childs;

            Node *parent = nullptr;
            size_type parentIndex = 0;

            Node() {}

            constexpr size_type elementSize() const { return elements.size(); }
            constexpr size_type childSize() const { return childs.size(); }

            void insertElement(size_type index, const T &obj) {
                elements.insert(index, obj);
                insertChild(index, nullptr);
            }

            void insertChild(size_type index, Node *node) {
                childs.insert(index, node);
            }

            bool hasChild(size_type index) const {
                if (childSize() <= index) {
                    return false;
                }
                return childs[index];
            }

            T &getElement(size_type index) {
                return elements[index];
            }

            Node *getChild(size_type index) const {
                return childs[index];
            }

            size_type findInsertingIndex(const T &obj) {
                // must find the place where the element is larger than obj
                // and the previous one is less than obj
                size_type left = 0, right = elementSize();
                if (right == 0) {
                    return 0;
                }
                while (true) {
                    if (left + 1 == right) {
                        if (obj <= getElement(left)) {
                            return left;
                        }
                        return right;
                    }
                    size_type mid = ((right - left) >> 1ul) + left;
                    T &midObj = getElement(mid);
                    // organize if-else in this way so that we only need operator < & ==
                    if (obj < midObj) {  // less than
                        right = mid;
                    } else if (obj == midObj) {  // equal
                        return mid;
                    } else {  // greater than
                        left = mid;
                    }
                }
            }

            void splitThis() {
                size_type length = elementSize();
                size_type midIndex = length / 2;
                size_type leftSize = midIndex, rightIndex = midIndex + 1, rightLength = length - rightIndex;

                if (parent == nullptr) {
                    parent = new Node;
                    parent->insertChild(0, this);
                }

                // middle element move up
                parent->elements.insert(parentIndex, move(getElement(midIndex)));

                // right node
                Node *newRight = new Node;
                // copy elements
                newRight->elements.insert(0, elements.begin_iterator() + rightIndex, rightLength);
                // copy childs
                newRight->childs.insert(0, childs.begin_iterator() + rightIndex, rightLength);
                // set up parent info
                newRight->parentIndex = parentIndex + 1;
                newRight->parent = parent;
                for (size_type index = parentIndex + 2; index < parent->childSize(); ++index) {
                    ++(parent->getChild(index)->parentIndex);
                }
                parent->insertChild(parentIndex + 1, newRight);
                // reset this
                elements.remove(midIndex, length - midIndex);
                childs.remove(rightIndex, rightLength);
            }
        };

        Node *root = new Node;

        bool needSplit(Node *node) const {
            return node->elementSize() > degree;
        }

    public:
        BTree() = default;

        void insert(const T &obj) {
            Node *node = root;
            // insert
            size_type insertIndex;
            while (true) {
                insertIndex = node->findInsertingIndex(obj);
                if (!node->hasChild(insertIndex)) {
                    break;
                }
                node = node->getChild(insertIndex);
            }
            node->insertElement(insertIndex, obj);

            // split
            while (needSplit(node)) {
                node->splitThis();
                node = node->parent;
            }
            // update root
            while (root->parent) {
                root = root->parent;
            }
        }

        bool hasMember(const T &obj) {
            Node *node = root;
            // insert
            size_type insertIndex;
            while (true) {
                insertIndex = node->findInsertingIndex(obj);
                if (node->getElement(insertIndex) == obj) {
                    return true;
                }
                if (!node->hasChild(insertIndex)) {
                    break;
                }
                node = node->getChild(insertIndex);
            }
            return obj == node->getElement(insertIndex);
        }
    };
}

#endif //ANBASE_BTREE_HPP
