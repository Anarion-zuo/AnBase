//
// Created by 左钰 on 2020/5/25.
//

#ifndef ANBASE_BTREE_HPP
#define ANBASE_BTREE_HPP

namespace anarion {
    template<typename T>
    class BTree {
    protected:
        const size_type maxDegree = 2;
        const size_type minDegree = 1;

        struct Node {
            Vector<T> elements;
            Vector<Node*> childs;

            Node *parent = nullptr;
            size_type parentIndex = 0;

            Node() = default;
            ~Node() {
                for (size_type index = 0; index < childs.size(); ++index) {
                    delete childs[index];
                }
            }

            constexpr size_type elementSize() const { return elements.size(); }
            constexpr size_type childSize() const { return childs.size(); }

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

            void splitRoot() {
                size_type length = elementSize();
                size_type midIndex = length / 2;
                size_type leftSize = midIndex, rightIndex = midIndex + 1, rightLength = length - rightIndex;

                T midObj = move(elements[midIndex]);

                // set up new node
                Node *newRight = new Node;
                newRight->elements.insert(0, elements.begin_iterator() + rightIndex, rightLength);
                newRight->childs.insert(0, childs.begin_iterator() + rightIndex, rightLength + 1);
                for (size_type index = 0; index < rightLength + 1; ++index) {
                    Node *child = newRight->childs[index];
                    if (child) {
                        child->parentIndex -= rightIndex;
                        child->parent = newRight;
                    }
                }

                // update this node
                elements.remove(midIndex, rightLength + 1);
                childs.remove(rightIndex, rightLength + 1);

                // set up new parent
                parent = new Node;
                parent->elements.push_back(move(midObj));
                parent->childs.push_back(this);
                parent->childs.push_back(newRight);
                newRight->parent = parent;
                newRight->parentIndex = 1;
                parentIndex = 0;
            }

            void splitNotRoot() {
                size_type length = elementSize();
                size_type midIndex = length / 2;
                size_type leftSize = midIndex, rightIndex = midIndex + 1, rightLength = length - rightIndex;

                T midObj = move(elements[midIndex]);

                // set up new node
                Node *newRight = new Node;
                newRight->elements.insert(0, elements.begin_iterator() + rightIndex, rightLength);
                newRight->childs.insert(0, childs.begin_iterator() + rightIndex, rightLength + 1);
                for (size_type index = 0; index < rightLength + 1; ++index) {
                    Node *child = newRight->childs[index];
                    if (child) {
                        child->parentIndex -= rightIndex;
                        child->parent = newRight;
                    }
                }

                // update this node
                elements.remove(midIndex, rightLength + 1);
                childs.remove(rightIndex, rightLength + 1);

                // set up new parent
                parent->elements.insert(parentIndex, move(midObj));
                parent->childs.insert(parentIndex + 1, newRight);
                for (size_type index = parentIndex + 2; index < parent->childSize(); ++index) {
                    ++(parent->childs[index]->parentIndex);
                }
                newRight->parent = parent;
                newRight->parentIndex = parentIndex + 1;
            }

            void splitThis() {
                if (parent == nullptr) {
                    splitRoot();
                } else {
                    splitNotRoot();
                }
            }
        };

        Node *root = new Node;

        bool needSplit(Node *node) const {
            return node->elementSize() > maxDegree;
        }

        Node *insertNoSplit(const T &obj) {
            Node *node = root;
            if (root->childs.empty()) {
                root->childs.push_back(nullptr);
            }
            // insert
            size_type insertIndex;
            while (true) {
                insertIndex = node->findInsertingIndex(obj);
                if (!node->hasChild(insertIndex)) {
                    break;
                }
                node = node->childs[insertIndex];
            }
            node->elements.insert(insertIndex, obj);
            node->childs.insert(insertIndex, nullptr);
            return node;
        }

        Node *insertNoSplit(T &&obj) {
            Node *node = root;
            if (root->childs.empty()) {
                root->childs.push_back(nullptr);
            }
            // insert
            size_type insertIndex;
            while (true) {
                insertIndex = node->findInsertingIndex(obj);
                if (!node->hasChild(insertIndex)) {
                    break;
                }
                node = node->childs[insertIndex];
            }
            node->elements.insert(insertIndex, forward<T>(obj));
            node->childs.insert(insertIndex, nullptr);
            return node;
        }

        void insertTrySplit(Node *node) {
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

        void removeCaseDirectRemove(Node *node, size_type index) {
            node->elements.remove(index);
        }
        void removeCaseTryBorrow(Node *node, size_type index) {
            // check neighbors for balancing
            size_type parentIndex = node->parentIndex;
            Node *parent = node->parent;

            size_type borrowingIndex;
            Node *borrowingNode;
            bool lr;  // left/right true/false
            // small case first
            if (parentIndex == 0) {  // can only look to the right
                borrowingNode = parent->childs[1];
                if (borrowingNode->elementSize() > 1) {
                    lr = true;
                    borrowingIndex = 0;
                } else {
                    // give up
                    removeCaseCannotBorrow(node, index);
                    return;
                }
            } else {
                borrowingNode = parent->childs[parentIndex - 1];  // look left first
                if (borrowingNode->elementSize() > 1) {
                    // viable
                    lr = false;
                    borrowingIndex = borrowingNode->elementSize() - 1;
                } else if (parentIndex + 1 < parent->childSize()) {  // check if can look right
                    borrowingNode = parent->childs[parentIndex + 1];
                    if (borrowingNode->elementSize() > 1) {
                        // viable
                        lr = true;
                        borrowingIndex = 0;
                    } else {
                        // give up
                        removeCaseCannotBorrow(node, index);
                        return;
                    }
                } else {
                    // give up
                    removeCaseCannotBorrow(node, index);
                    return;
                }
            }
            // can borrow node from neighbor
            T borrowedObj = move(borrowingNode->elements[borrowingIndex]);
            borrowingNode->elements.remove(borrowingIndex);
            if (lr) {
                // is from right
                node->elements.push_back(move(borrowedObj));
            } else {
                // is from left
                node->elements.insert(0, move(borrowedObj));
            }
            node->childs.push_back(nullptr);
        }
        void removeCaseCannotBorrow(Node *node, size_type index) {

        }
        void removeCaseNotLeaf(Node *node, size_type index) {

        }

    public:
        BTree() = default;

        void insert(const T &obj) {
            insertTrySplit(insertNoSplit(obj));
        }

        void insert(T &&obj) {
            insertTrySplit(insertNoSplit(forward<T>(obj)));
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

        void remove(const T &obj) {
            Node *node = root;
            // find the node
            size_type insertIndex;
            while (true) {
                insertIndex = node->findInsertingIndex(obj);
                if (node->getElement(insertIndex) == obj) {
                    break;
                }
                if (!node->hasChild(insertIndex)) {
                    break;
                }
                node = node->getChild(insertIndex);
            }

            if (node->childs.empty()) {  // leaf node
                if (node->elements.size() > 1) {
                    // case 1
                    removeCaseDirectRemove(node, insertIndex);
                    return;
                }
                // case 2 & 3
                removeCaseTryBorrow(node, insertIndex);
                return;
            }
            // case 3
            removeCaseCannotBorrow(node, insertIndex);
        }
    };
}

#endif //ANBASE_BTREE_HPP
