//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_FREELIST_HPP
#define ANBASE_FREELIST_HPP

#include <container/List/Vector.hpp>

namespace anarion {
    /**
     * @brief Accounts for object occupation information.
     * @tparam index_t numerical type of the index.
     * @details Implements a free list data structure for objects identified by numerical indeices. Maintains an array of indices. By pointer index mechanism [], one may find the `next` element's index of the previous one.
     *
     * Array sequence in (1 2 3 4) suggests a linked list of indices to be [0->1->2->3->], 4 being the length of the array marking the end of the linked list.
     *
     * end-of-list is noted by the length value of the array, or element of the array pointing to itself.
     */
    template <typename index_t>
    class FreeList {
        Vector<index_t> list;
        index_t head;

        static const index_t null;

        /**
         * @brief Initializes a linked list on a continuous region of address.
         * @param begin
         * @param length
         */
        void makeContinuous(index_t begin, size_type length) {
            for (index_t index = begin; index - begin < length - 1; ++index) {
                list.get(index) = index + 1;
            }
        }

        constexpr void setNextNull(index_t index) {
            list.get(index) = null;
        }

        constexpr bool isNull(index_t index) const {
            return list.get(index) == null;
        }

    public:

        explicit FreeList(index_t initLength) : list(initLength, null), head(0) {
            makeContinuous(0, initLength);
            list.get(initLength - 1) = head;
        }

        FreeList(const FreeList<index_t> &) = default;
        FreeList(FreeList<index_t> &&) noexcept = default;
        ~FreeList() = default;

        /**
         * @return size of the accounted object pool.
         */
        index_t size() const { return list.size(); }

        /**
         * @brief checks whether the object in the pool indexed by the index is marked used.
         * @param index
         * @return
         * @details
         */
        bool isUsed(index_t index) const {
            return isNull(index);
        }

        /**
         * @brief checks whether the object pool is empty.
         * @return
         */
        bool isEmpty() const {
            return isUsed(head);
        }

        /**
         * @brief fetches a free object from the pool.
         * @param ret returned parameter.
         * @return whether the operation was successful.
         */
        bool fetch(index_t &ret) {
            if (isEmpty()) {
                ret = null;
                return false;
            }
            // return value
            ret = head;
            // move to next
            head = list.get(head);
            // mark used
            setNextNull(ret);
            return true;
        }

        /**
         * @brief return an previously allocated object back to the pool.
         * @param element element to be returned.
         * @return whether the operation was successful.
         */
        bool putBack(index_t element) {
            if (!isUsed(element)) {
                return false;
            }
            // push linked list
            list.get(element) = head;
            head = element;
            return true;
        }

        /**
         * @brief Expands pool size.
         * @param count
         */
        void append(index_t count) {
            index_t newBegin = size();
            list.resize(newBegin + count);
            for (size_type index = 0; index < count; ++index) {
                list.pushBack(null);
            }
            makeContinuous(newBegin, count);
            list.get(count + newBegin - 1) = head;
            list.get(list.size() - 1) = head;
            head = newBegin;
        }

        /**
         * @brief Shrink the array at the end.
         * @param count how much to shrink.
         * @details Must go through the free list to tie up loose ends.
         */
        void shrink(index_t count) {
            if (count == size()) {
                list.clear();
                return;
            }
            // move head out of the removing range
            while (head >= size() - count) {
                head = list.get(head);
            }
            index_t cur = head, enter = -1;
            while (cur != null) {
                index_t next = list.get(cur);
                if (next >= size() - count) {
                    enter = cur;
                } else {
                    if (enter != static_cast<index_t>(-1)) {
                        list.get(enter) = next;
                        enter = -1;
                    }
                }
                cur = next;
            }
            // resize
            list.resize(size() - count);
        }
    };
    template <typename index_t>
    const index_t FreeList<index_t>::null = -1;
}

#endif //ANBASE_FREELIST_HPP
