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
//                for (size_type index = 0; index < childs.size(); ++index) {
//                    delete childs[index];
//                }
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
                size_type left = 0, right = elements.size();
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

        void removeCaseDirectRemove(Node *node, size_type index, Node *parent, size_type parentIndex) {
            node->elements.remove(index);
            node->childs.remove(index);
        }
        void removeRotateLeft(Node *node, Node *parent, Node *sibling, size_type parentIndex) {
            node->elements.push_back(move(parent->elements[parentIndex]));
            parent->elements[parentIndex] = sibling->elements.pop_front();
            sibling->childs.pop_back();
        }
        void removeRotateRight(Node *node, Node *parent, Node *sibling, size_type parentIndex) {
            node->elements.push_back(move(parent->elements[parentIndex - 1]));
            parent->elements[parentIndex] = sibling->elements.pop_back();
            sibling->childs.pop_back();
        }
        void removeMergeLeaf(Node *left, Node *right, Node *parent) {
            size_type parentIndex = left->parentIndex;
            // change left to new node
            left->elements.push_back(move(parent->elements[parentIndex]));   // add middle element of parent to left
            parent->elements.remove(parentIndex);
            parent->childs.remove(parentIndex + 1);  // right removed from parent childs
            // update parent childs' index
            for (size_type index = parentIndex + 1; index < parent->childs.size(); ++index) {
                --(parent->childs[index]->parentIndex);
            }
            // elements move from right to left
            left->elements.insert(left->elements.end_iterator(), right->elements.begin_iterator(), right->elements.size());
            // update new moved childs
            for (size_type index = 0; index < right->childs.size(); ++index) {
                Node *child = right->childs[index];
                if (child) {
                    child->parentIndex += left->childs.size();
                    child->parent = left;
                }
            }
            // childs move from right to left
            left->childs.insert(left->childs.end_iterator(), right->childs.begin_iterator(), right->childs.size());
            delete right;
            // check if must fill in the parent
            if (parent->elements.empty()) {
                removeFillEmpty(parent, parent->parent, parentIndex);
            }
        }
        void removeFillEmpty(Node *node, Node *parent, size_type parentIndex) {
            if (parent == nullptr) {  // root case
                // set root's child to be root
                Node *child = node->childs[parentIndex];
                child->parentIndex = 0;
                child->parent = nullptr;
                root = child;
                delete node;
                return;
            }
            if (parent->childs.size() - 1 > parentIndex) {  // has right sibling
                Node *sibling = parent->childs[parentIndex + 1];
                if (sibling->elements.size() > 1) {  // can borrow from right sibling
                    removeRotateLeft(node, parent, sibling, parentIndex);
                    return;
                }
                // check left sibling
                if (parentIndex > 0) {
                    sibling = parent->childs[parentIndex - 1];
                    if (sibling->elements.size() > 1) {
                        removeRotateRight(node, parent, sibling, parentIndex);
                        return;
                    }
                    // must merge
                    removeMergeLeaf(sibling, node, parent);
                    return;
                }
                // must merge
                removeMergeLeaf(node, sibling, parent);
                return;
            }
            // no right sibling
            Node *sibling = parent->childs[parentIndex - 1];;
            if (sibling->elements.size() > 1) {
                removeRotateRight(node, parent, sibling, parentIndex);
                return;
            }
            removeMergeLeaf(sibling, node, parent);
        }
        void removeCaseNotLeaf(Node *node, size_type index) {
            T removingElement = move(node->elements[index]);
            Node *leaf = node;
            while (leaf->childs[index] != nullptr) {
                Node *next = leaf->childs[index];
                next->parent->elements[index] = next->elements.pop_back();
                leaf = next;
                index = next->elements.size();
            }
            leaf->childs.pop_back();
            if (leaf->elements.empty()) {
                removeFillEmpty(leaf, leaf->parent, leaf->parentIndex);
            }
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
                if (node->elements.size() > insertIndex) {
                    if (node->elements.get(insertIndex) == obj) {
                        return true;
                    }
                }

                if (!node->hasChild(insertIndex)) {
                    break;
                }
                node = node->getChild(insertIndex);
            }
            if (node->elements.size() > insertIndex) {
                if (node->elements.get(insertIndex) == obj) {
                    return true;
                }
            }
            return false;
        }

        void remove(const T &obj) {
            Node *node = root;
            // find the node
            size_type removeIndex;
            bool foundNode = false;
            while (true) {
                removeIndex = node->findInsertingIndex(obj);
                if (node->elements.size() > removeIndex) {
                    if (node->elements.get(removeIndex) == obj) {
                        foundNode = true;
                        break;
                    }
                }
                if (!node->hasChild(removeIndex)) {
                    break;
                }
                node = node->getChild(removeIndex);
            }
            if (!foundNode) { return; }
            size_type parentIndex = node->parentIndex;
            Node *parent = node->parent;
            if (!node->hasChild(removeIndex)) {  // leaf node
                // perfect leaf, remove directly
                removeCaseDirectRemove(node, removeIndex, parent, parentIndex);
                if (node->elements.empty()) {
                    removeFillEmpty(node, parent, parentIndex);
                }
                return;
            }
            // internal node
            removeCaseNotLeaf(node, removeIndex);
        }
    };
}

#endif //ANBASE_BTREE_HPP
