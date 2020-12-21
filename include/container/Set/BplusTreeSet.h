//
// Created by anarion on 8/11/20.
//

#ifndef ANBASE_BPLUSTREESET_H
#define ANBASE_BPLUSTREESET_H

#include <container/List/LinkedList.hpp>
#include "../List/Vector.hpp"

namespace anarion {
    template <typename T>
    struct EntryGetThisCaller {
        const T &get(const T &obj) { return obj; }
    };
    template <typename T, typename K>
    struct EntryGetKeyCaller {
        const K &get(const T &obj) { return obj.get_key(); }
    };
    // implements a b plus tree
    // type T must override < operator
    template <typename T, size_type levelSize = 4, typename K = T, typename keyGetter = EntryGetThisCaller<T>>
    class BplusTreeSet {
        friend class TestBplusTreeSet;
    protected:
        struct MiddleNode;
        struct Node {
//            Vector<T> objArray;
            MiddleNode *parent = nullptr;
            virtual bool isLeaf() const = 0;

            Node() = default;

            enum BinSearchState { equal, larger, less };

            virtual void removeChilds() = 0;

            static enum BinSearchState binSearchKey(const K &obj, size_type &index, const Vector<K> &objArray) {
                size_type left = 0, right = objArray.size(), middle;
                if (left == right) {
                    index = left;
                    return BinSearchState::equal;
                }
                while (true) {
                    middle = ((right - left) >> 1u) + left;
                    const K &middleObj = objArray.get(middle);
                    if (middleObj < obj) {
                        if (left + 1 >= right) {
                            index = left;
                            return BinSearchState::less;
                        }
                        left = middle;
                    } else if (middleObj == obj) {
                        index = middle;
                        return BinSearchState::equal;
                    } else {
                        if (left + 1 >= right) {
                            index = left;
                            return BinSearchState::larger;
                        }
                        right = middle;
                    }
                }
            }
            static enum BinSearchState binSearchObj(const K &obj, size_type &index, const Vector<T> &objArray) {
                size_type left = 0, right = objArray.size(), middle;
                if (left == right) {
                    index = left;
                    return BinSearchState::equal;
                }
                while (true) {
                    middle = ((right - left) >> 1u) + left;
                    const K &middleObj = keyGetter().get(objArray.get(middle));
                    if (middleObj < obj) {
                        if (left + 1 >= right) {
                            index = left;
                            return BinSearchState::less;
                        }
                        left = middle;
                    } else if (middleObj == obj) {
                        index = middle;
                        return BinSearchState::equal;
                    } else {
                        if (left + 1 >= right) {
                            index = left;
                            return BinSearchState::larger;
                        }
                        right = middle;
                    }
                }
            }
            /*
             * Find the right index to insert obj.
             * If there is an element which equals obj, the function returns its index.
             * If there is not, the function returns the index of the smallest element larger than obj.
             */
            /*
            size_type findInsertingIndex(const T &obj) const {
                size_type result;
                BinSearchState state = binSearch(obj, result, objArray);
                if (state == BinSearchState::less) {
                    ++result;
                }
                return result;
            }
             */
            // splitting
            template <typename Type>
            static bool needsSplit(const Vector<Type> &objArray) {
                return objArray.size() > levelSize;
            }
            template <typename Type>
            static void computeSpltSizes(size_type &thisSize, size_type &newSize, const Vector<Type> &objArray) {
                size_type oldSize = objArray.size();
                newSize = oldSize / 2;
                thisSize = oldSize - newSize;
            }
            template <typename ThisArrayType, typename NewArrayType>
            static void insertRightObjsToNewNode(Vector<NewArrayType> &newNodeObjArray, size_type thisSize, size_type newSize, const Vector<ThisArrayType> &objArray) {
                newNodeObjArray.insert(0ul, objArray.beginIterator() + thisSize, newSize);
            }
//            virtual void removeMovedNode(size_type thisSize, size_type newSize) = 0;
//            virtual NFANode *createNewNode() = 0;
//            virtual void moveChildsToNewNode(NFANode *newNode, size_type thisSize, size_type newSize) = 0;
            size_type getParentIndex() const {
                for (size_type index = 0; index < parent->childsArray.size(); ++index) {
                    if (parent->childsArray.get(index) == this) {
                        return index;
                    }
                }
            }

            MiddleNode *setupNewRootAsParentKey(Node *newNode, size_type thisSize, Vector<K> &objArray) {
                MiddleNode *newRoot = new MiddleNode;
                this->parent = newRoot;
                newNode->parent = this->parent;
                // insert obj
                this->parent->objArray.pushBack(objArray.get(thisSize - 1));
                // insert child
                this->parent->childsArray.pushBack(this);
                this->parent->childsArray.pushBack(newNode);
                return newRoot;
            }
            MiddleNode *setupNewRootAsParentObj(Node *newNode, size_type thisSize, Vector<T> &objArray) {
                MiddleNode *newRoot = new MiddleNode;
                this->parent = newRoot;
                newNode->parent = this->parent;
                // insert obj
                this->parent->objArray.pushBack(keyGetter().get(objArray.get(thisSize - 1)));
                // insert child
                this->parent->childsArray.pushBack(this);
                this->parent->childsArray.pushBack(newNode);
                return newRoot;
            }

//            virtual void updateLinkedList(NFANode *newNode) = 0;
            virtual MiddleNode *split() = 0;
            virtual bool needsSplit() const = 0;
            /*
            MiddleNode *split() {
                // prepare
                size_type thisSize, newSize;
                computeSpltSizes(thisSize, newSize);
                NFANode *newNode = createNewNode();
                // objs
                insertRightObjsToNewNode(newNode, thisSize, newSize);
                // possible childs
                moveChildsToNewNode(newNode, thisSize, newSize);
                // parents
                MiddleNode *result = nullptr;
                if (parent) {
                    setupParent(getParentIndex(), newNode, thisSize);
                } else {
                    result = setupNewRootAsParent(newNode, thisSize);
                }
                // remove now
                removeMovedNode(thisSize, newSize);
                // possible linked list updates
                updateLinkedList(newNode);
                return result;
            }
             */
            template <typename Type>
            static void print(const Vector<Type> &objArray) {
                printf("#");
                for (auto it = objArray.beginIterator(); it != objArray.endIterator(); ++it) {
                    printf("%d ", *it);
                }
                printf("#");
            }
        };
        struct MiddleNode : public Node {
            Vector<Node*> childsArray;
            Vector<K> objArray;

            MiddleNode() : childsArray(levelSize + 2) {}

            bool isLeaf() const override {
                return false;
            }

            void removeMovedNode(size_type thisSize, size_type newSize) {
                objArray.remove(thisSize - 1, newSize + 1);
            }

            void setupParent(size_type myParentIndex, Node *newNode, size_type thisSize) {
                newNode->parent = this->parent;
                // insert obj
                this->parent->objArray.insert(myParentIndex, objArray.get(thisSize - 1));
                // insert child
                this->parent->childsArray.insert(myParentIndex + 1, newNode);
            }

            void moveChildsToNewNode(Node *newNode, size_type thisSize, size_type newSize) {
                MiddleNode *newMiddleNode = dynamic_cast<MiddleNode*>(newNode);
                // set parent pointers first
                for (size_type index = 0; index <= newSize; ++index) {
                    childsArray.get(thisSize + index)->parent = newMiddleNode;
                }
                // insert pointers into newNode
                // be extra careful with this 1
                newMiddleNode->childsArray.insert(0ul, childsArray.beginIterator() + thisSize, newSize + 1);
                childsArray.remove(thisSize, newSize + 1);
            }
/*
            void updateLinkedList(NFANode *newNode) override {
                // nothing here
            }
*/
            bool needsSplit() const { return Node::needsSplit(objArray); }
            MiddleNode *split() override {
                // prepare
                size_type thisSize, newSize;
                Node::computeSpltSizes(thisSize, newSize, objArray);
                MiddleNode *newNode = new MiddleNode;
                // objs
                Node::insertRightObjsToNewNode(newNode->objArray, thisSize, newSize, objArray);
                // possible childs
                moveChildsToNewNode(newNode, thisSize, newSize);
                // parents
                MiddleNode *result = nullptr;
                if (this->parent) {
                    setupParent(this->getParentIndex(), newNode, thisSize);
                } else {
                    result = this->setupNewRootAsParentKey(newNode, thisSize, objArray);
                }
                // remove now
                removeMovedNode(thisSize, newSize);
                // possible linked list updates
//                updateLinkedList(newNode);
                return result;
            }

            void removeChilds() override {
                for (auto it = childsArray.beginIterator(); it != childsArray.endIterator(); ++it) {
                    (**it).removeChilds();
                    delete *it;
                }
            }
        };
        struct LeafNode : public Node {
            LeafNode *prev = nullptr, *next = nullptr;
            Vector<T> objArray;

            bool isLeaf() const override {
                return true;
            }

            void removeMovedNode(size_type thisSize, size_type newSize) {
                this->objArray.remove(thisSize, newSize);
            }
/*
            void moveChildsToNewNode(NFANode *newNode, size_type thisSize, size_type newSize) {
                // nothing here
            }
*/
            void updateLinkedList(Node *newNode) {
                LeafNode *newLeafNode = dynamic_cast<LeafNode*>(newNode), *oldNext = this->next;
                next = newLeafNode;
                newLeafNode->prev = this;
                newLeafNode->next = oldNext;
                oldNext->prev = newLeafNode;
            }
            void setupParent(size_type myParentIndex, Node *newNode, size_type thisSize) {
                newNode->parent = this->parent;
                // insert obj
                this->parent->objArray.insert(myParentIndex, keyGetter().get(objArray.get(thisSize - 1)));
                // insert child
                this->parent->childsArray.insert(myParentIndex + 1, newNode);
            }

            bool needsSplit() const { return Node::needsSplit(objArray); }
            MiddleNode *split() override {
                // prepare
                size_type thisSize, newSize;
                Node::computeSpltSizes(thisSize, newSize, objArray);
                LeafNode *newNode = new LeafNode;
                // objs
                Node::insertRightObjsToNewNode(newNode->objArray, thisSize, newSize, objArray);
                // possible childs
//                moveChildsToNewNode(newNode, thisSize, newSize);
                // parents
                MiddleNode *result = nullptr;
                if (this->parent) {
                    setupParent(this->getParentIndex(), newNode, thisSize);
                } else {
                    result = this->setupNewRootAsParentObj(newNode, thisSize, objArray);
                }
                // remove now
                removeMovedNode(thisSize, newSize);
                // possible linked list updates
                updateLinkedList(newNode);
                return result;
            }

            void removeChilds() override {
                // nothing here
            }
        };

        Node *root;
        LeafNode stub;             // making up doubly linked list
        size_type objCount;

        void init() {
            LeafNode *rootNode = new LeafNode();
            stub.next = rootNode;
            stub.prev = rootNode;
            rootNode->prev = &stub;
            rootNode->next = &stub;
            root = rootNode;
            objCount = 0;
        }

        void destruct() {
            root->removeChilds();
            delete root;
        }

    public:

        BplusTreeSet() {
            init();
        }

        void clear() {
            destruct();
            init();
        }

        ~BplusTreeSet() { destruct(); }

    protected:
        void fetchLeafNodeKey (const K &key, LeafNode *&leafNode, size_type &leafIndex) const {
            Node *node = root;
            enum Node::BinSearchState searchState;
            size_type index;
            while (true) {
                if (node->isLeaf()) {
                    searchState = node->binSearchObj(key, index, dynamic_cast<LeafNode *>(node)->objArray);
                    break;
                } else {
                    searchState = node->binSearchKey(key, index, dynamic_cast<MiddleNode *>(node)->objArray);
                }
                Vector<Node*> &childs = dynamic_cast<MiddleNode*>(node)->childsArray;
                // trace according to search state
                switch (searchState) {
                    case Node::BinSearchState::less:
                        node = childs.get(index + 1);
                        break;
                    case Node::BinSearchState::equal:
                    case Node::BinSearchState::larger:
                        node = childs.get(index);
                        break;
                }
            }
            leafNode = dynamic_cast<LeafNode*>(node);
            switch (searchState) {
                case Node::BinSearchState::less:
                    leafIndex = index + 1;
                    break;
                case Node::BinSearchState::equal:
                case Node::BinSearchState::larger:
                    leafIndex = index;
                    break;
            }
        }
        void fetchLeafNodeObj(const T &obj, LeafNode *&leafNode, size_type &leafIndex) const {
            const K &key = keyGetter().get(obj);
            fetchLeafNodeKey(key, leafNode, leafIndex);
        }
        void insertTraceUp(Node *node) {
            while (true) {
                if (!node->needsSplit()) {
                    break;
                }
                MiddleNode *newRoot = node->split();
                if (newRoot) {
                    root = newRoot;
                    break;
                }
                node = node->parent;
            }
        }

    public:

        void insert(const T &obj) {
            size_type leafIndex;
            LeafNode *leafNode;
            fetchLeafNodeObj(obj, leafNode, leafIndex);
            leafNode->objArray.insert(leafIndex, obj);
            insertTraceUp(leafNode);
            ++objCount;
            testChildParentPointers();
        }
        bool has(const T &obj) const {
            if (objCount == 0) {
                return false;
            }
            size_type leafIndex;
            LeafNode *leafNode;
            fetchLeafNodeObj(obj, leafNode, leafIndex);
            if (leafIndex > leafNode->objArray.size()) {
                return false;
            }
            if (leafIndex == leafNode->objArray.size()) {
                return leafNode->next->objArray.get(0) == obj;
            }
            return keyGetter().get(leafNode->objArray.get(leafIndex)) == keyGetter().get(obj);
        }
        void print() const {
            LinkedList<Node*> queue;
            queue.push_front(root);
            while (!queue.empty()) {
                Node *node = queue.pop_back();
                if (node->isLeaf()) {
                    Node::print(dynamic_cast<LeafNode*>(node)->objArray);
                } else {
                    Node::print(dynamic_cast<MiddleNode*>(node)->objArray);
                }
                printf("\n");
                if (!node->isLeaf()) {
                    Vector<Node*> &childs = dynamic_cast<MiddleNode*>(node)->childsArray;
                    for (auto it = childs.beginIterator(); it != childs.endIterator(); ++it) {
                        queue.push_front(*it);
                    }
                }
//                printf("\n");
            }
        }

        /*
         * Iterators
         *
         * Iterates from least to largest.
         */
        class iterator {
            friend class BplusTreeSet;
        protected:
            LeafNode *curNode;
            T *vectorIterator;
        public:
            iterator() = default;
            explicit iterator(LeafNode *curNode) : curNode(curNode), vectorIterator(curNode->objArray.beginIterator()) {}
            ~iterator() = default;
            iterator(const iterator &rhs) = default;
            iterator &operator=(const iterator &) = default;

            const T &operator*() { return *vectorIterator; }
            const T *operator->() const { return vectorIterator; }

            iterator &operator++() {
                if (vectorIterator == nullptr) {
                    // is endIterator
                    curNode = curNode->next;
                    vectorIterator = curNode->objArray.beginIterator();
                    return *this;
                }
                ++vectorIterator;
                if (vectorIterator == curNode->objArray.endIterator()) {
                    curNode = curNode->next;
                    vectorIterator = curNode->objArray.beginIterator();
                }
                return *this;
            }

            iterator operator++(int) {
                iterator result(*this);
                ++(*this);
                return result;
            }

            iterator &operator--() {
                if (vectorIterator == curNode->objArray.beginIterator()) {
                    curNode = curNode->prev;
                    T *endIt = curNode->objArray.endIterator();
                    if (endIt == curNode->objArray.beginIterator()) {
                        vectorIterator = endIt;
                        return *this;
                    }
                    vectorIterator = endIt - 1;
                } else {
                    --vectorIterator;
                }
                return *this;
            }

            iterator operator--(int) {
                iterator result(*this);
                --(*this);
                return result;
            }

            bool operator==(const iterator &rhs) const {
                return curNode == rhs.curNode &&
                       vectorIterator == rhs.vectorIterator;
            }

            bool operator!=(const iterator &rhs) const {
                return !(rhs == *this);
            }

            iterator &operator+=(size_type steps) {
                size_type nodeMore = curNode->objArray.endIterator() - vectorIterator;
                while (steps >= nodeMore) {
                    curNode = curNode->next;
                    vectorIterator = curNode->objArray.beginIterator();
                    steps -= nodeMore;
                    nodeMore = curNode->objArray.size();
                }
                vectorIterator += steps;
                return *this;
            }

            iterator &operator-=(size_type steps) {
                size_type nodeMore = vectorIterator - curNode->objArray.beginIterator();
                while (steps > nodeMore) {
                    curNode = curNode->prev;
                    vectorIterator = curNode->objArray.endIterator();
                    steps -= nodeMore;
                    nodeMore = curNode->objArray.size();
                }
                vectorIterator -= steps;
                return *this;
            }

        };

        iterator begin_iterator() {
            return iterator(stub.next);
        }
        iterator end_iterator() {
            return iterator(&stub);
        }

        /**
         * @details Finding function returns an iterator.
         * Returns the position where the element should be inserted,
         * which is the position the element less than or equal to the input obj.
         */
        iterator find(const K &obj) {
            if (objCount == 0) {
                return end_iterator();
            }
            size_type leafIndex;
            LeafNode *leafNode;
            fetchLeafNodeKey(obj, leafNode, leafIndex);
            if (leafIndex > leafNode->objArray.size()) {
                return end_iterator();
            }
            iterator result(leafNode);
            result += leafIndex;
            return result;
        }

    protected:
        void removeTraceUp(MiddleNode *node, size_type nodeIndex) {
            node->objArray.remove(nodeIndex);
            node->childsArray.remove(nodeIndex);
            if (node->objArray.empty()) {
            }
        }
    public:

        void remove(iterator &it) {
            if (objCount == 0) {
                return;
            }
            if (it == end_iterator()) {
                return;
            }
            it.curNode->objArray.remove(it.vectorIterator);
            if (it.curNode->objArray.empty()) {
                MiddleNode *parent = it.curNode->parent;
                size_type parentIndex = it.curNode->getParentIndex();
                removeTraceUp(parent, parentIndex);
            }
        }

        void remove(const K &key) {
            auto it = find(key);
            if (!(keyGetter().get(*it) == key)) {
                return;
            }
            remove(it);
        }

        /*
         * Tests
         */
        bool testChildParentPointers() const {
            LinkedList<Node*> queue;
            queue.push_front(root);
            while (!queue.empty()) {
                Node *node = queue.pop_back();
                if (!node->isLeaf()) {
                    Vector<Node*> &childs = dynamic_cast<MiddleNode*>(node)->childsArray;
                    for (auto it = childs.beginIterator(); it != childs.endIterator(); ++it) {
                        if ((**it).parent != node) {
                            printf("This node has a child with parent not pointing to this node.\n");
                            printf("This node address: %lx, This Child index: %lu, This Child's parent: %lx.\n", this, it -
                                    childs.beginIterator(), (**it).parent);
                            printf("This node content: ");
                            Node::print(dynamic_cast<MiddleNode*>(node)->objArray);
                            printf("Faulty parent node content: ");
                            Node::print((**it).parent->objArray);
                            printf("\n");
                            return false;
                        }
                        queue.push_front(*it);
                    }
                }
//                printf("\n");
            }
            return true;
        }
    };
}

#endif //ANBASE_BPLUSTREESET_H
