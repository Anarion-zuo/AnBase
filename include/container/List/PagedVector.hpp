//
// Created by 左钰 on 2020/5/30.
//

#ifndef ANBASE_DEQUE_H
#define ANBASE_DEQUE_H

namespace anarion {
    template <typename T, size_type nodeLength = 20>
    class PagedVector {
    protected:
        struct Node {
            T *arr= nullptr;

            Node() : arr(operator new(nodeLength * sizeof(T))) {}
            Node(Node &&rhs) noexcept : arr(rhs.arr) { rhs.arr = nullptr; }
            ~Node() {
                operator delete(arr, nodeLength * sizeof(T));
            }

            void clearIndex(size_type index) {
                arr[index].~T();
            }

            void clearUntil(size_type length) {
                for (size_type index = 0; index < nodeLength; ++index) {
                    clearIndex(index);
                }
            }

            void insert(size_type index, const T &obj) {
                new (arr + index) T(obj);
            }

            void insert(size_type index, T &&obj) {
                new (arr + index) T(forward<T>(obj));
            }

            Node copyUntil(size_type length) {
                Node ret;
                seq_copy(ret.arr, arr, length);
                return ret;
            }
        };

        Vector<Node> nodes;
        size_type curVectorOffset = 0;
        size_type curNodeOffset = 0;

    public:
        PagedVector() = default;
        PagedVector(const PagedVector<T> &rhs) : nodes(rhs.nodes.size()), curVectorOffset(rhs.curVectorOffset), curNodeOffset(rhs.curNodeOffset) {
            for (size_type index = 0; index < curVectorOffset; ++index) {
                nodes.push_back(rhs.nodes[index].copyUntil(nodeLength));
            }
            nodes.push_back(rhs.nodes[index].copyUntil(curNodeOffset));
        }
        PagedVector(PagedVector<T> &&rhs) noexcept : nodes(move(rhs.nodes)), curVectorOffset(rhs.curVectorOffset), curNodeOffset(rhs.curNodeOffset) {
            curNodeOffset = 0;
            curVectorOffset = 0;
        }
        ~PagedVector() {
            clear();
        }

        void clear() {
            for (size_type index = 0; index < curVectorOffset; ++index) {
                nodes[index].clearUntil(nodeLength);
            }
            nodes[curNodeOffset].clearUntil(curNodeOffset);
            nodes.clear();
            curNodeOffset = 0;
            curVectorOffset = 0;
        }

        void push_back
    };
}

#endif //ANBASE_DEQUE_H
