//
// Created by 左钰 on 2020/1/23.
//

#ifndef MYCPPLIB_LINKED_LIST_HPP
#define MYCPPLIB_LINKED_LIST_HPP

#include "../base/iterator_traits.hpp"
#include "../base/container_utility.hpp"

namespace anarion {
    template<typename T>
    class LinkedList {
    protected:
        struct list_node {
            T obj;
            list_node *next, *prev;

            list_node() : next(nullptr), prev(nullptr) {}

            explicit list_node(const T &o, list_node *next = nullptr, list_node *prev = nullptr) : obj(o), next(next), prev(prev) {}
            explicit list_node(T &&o, list_node *next = nullptr, list_node *prev = nullptr) : obj(forward<T>(o)), next(next), prev(prev) {}
            list_node(const list_node &rhs) : obj(rhs.obj), next(nullptr), prev(nullptr) {}
            list_node(list_node &&rhs) noexcept : obj(move(rhs.obj)), next(nullptr), prev(nullptr) {}
        };

        static void copy_list(list_node *begin, list_node *stop, list_node *rethead) {
            list_node h, *node = begin, *newnode = &h;
            while (node != stop->next) {
                newnode->next = new list_node(node->obj, nullptr, newnode);
                newnode = newnode->next;
                node = node->next;
            }
            rethead->next = h.next;
            rethead->prev = newnode;
            h.next->prev = rethead;
            newnode->next = rethead;
        }

        static void del_list(list_node *begin, list_node *stop) {
            list_node *node = begin;
            while (node != stop->next) {
                list_node *next = node->next;
                delete (node);
                node = next;
            }
        }

        list_node head;
        size_type count = 0;

        void insert_before(list_node *node, const T &o) {
            ++count;
            list_node *prev = node->prev, *newnode = new list_node(o, node, prev);
            node->prev = newnode;
            prev->next = newnode;
        }

        void insert_before(list_node *node, T &&o) {
            ++count;
            list_node *prev = node->prev, *newnode = new list_node(forward(o), node, prev);
            node->prev = newnode;
            prev->next = newnode;
        }

    public:

        // swap in-place
        static void swap_nodes(list_node *x, list_node *y) {
            list_node *xnext = x->next, *xprev = x->prev;
            x->next = y->next;
            x->prev = y->prev;
            y->next = xnext;
            y->prev = xprev;
        }

        friend class iterator;
        class iterator {
        protected:
            list_node *node;
        public:
            explicit iterator(list_node *node) : node(node) {}

            // traits
            typedef seq_iterator category;
            typedef long dif_type;
            typedef T val_type;
            typedef T* ptr_type;
            typedef T& ref_type;
            typedef T&& rval_type;

            bool operator==(const iterator &rhs) const {
                return node == rhs.node;
            }

            bool operator!=(const iterator &rhs) const {
                return !(rhs == *this);
            }

            T& operator*() {
                return node->obj;
            }

            T *operator->() {
                return &node->obj;
            }

            iterator &operator++() {
                node = node->next;
                return *this;
            }

            iterator operator++(int) {
                iterator result(*this);
                ++(*this);
                return result;
            }

            iterator &operator--() {
                node = node->prev;
                return *this;
            }

            iterator operator--(int) {
                iterator result(*this);
                --(*this);
                return result;
            }

            iterator operator+(long num) {
                if (num == 0) {
                    return *this;
                }
                if (num > 0) {
                    list_node *n = node;
                    for (long i = 0; i < num; ++i) {
                        n = n->next;
                    }
                    return iterator(n);
                }
                return operator-(-num);
            }

            iterator operator-(long num) {
                if (num == 0) {
                    return *this;
                }
                if (num > 0) {
                    list_node *n = node;
                    for (long i = 0; i < num; ++i) {
                        n = n->prev;
                    }
                    return iterator(n);
                }
                return operator+(-num);
            }

            long operator-(iterator rhs) {
                if (rhs.node == node) {
                    return 0;
                }
                list_node *n = rhs.node;
                long index = 0;
                while (n != node) {
                    ++index;
                    n = n->next;
                }
                return index;
            }

        };

        LinkedList() {
            head.prev = &head;
            head.next = &head;
        }

        LinkedList(const LinkedList<T> &rhs) : count(rhs.count) {
            if (count == 0) {
                return;
            }
            copy_list(rhs.head.next, rhs.head.prev, &head);
        }

        LinkedList(LinkedList<T> &&rhs) noexcept : count(rhs.count) {
            head.next = rhs.head.next;
            head.prev = rhs.head.prev;
            head.next->prev = &head;
            head.prev->next = &head;
            rhs.head.next = &rhs.head;
            rhs.head.prev = &rhs.head;
            rhs.count = 0;
        }

        LinkedList<T> &operator=(const LinkedList<T> &rhs) {
            if (&rhs == this) {
                return *this;
            }
            clear();
            if (rhs.count == 0) {
                return *this;
            }
            copy_list(rhs.head.next, rhs.head.prev, &head);
            return *this;
        }

        LinkedList<T> &operator=(LinkedList<T> &&rhs) noexcept {
            clear();
            count = rhs.count;
            if (count == 0) {
                return *this;
            }
            head.next = rhs.head.next;
            head.prev = rhs.head.prev;
            head.next->prev = &head;
            head.prev->next = &head;
            rhs.head.next = &rhs.head;
            rhs.head.prev = &rhs.head;
            rhs.count = 0;
            return *this;
        }

        ~LinkedList() {
            clear();
        }

        void clear() {
            count = 0;
            del_list(head.next, head.prev);
            head.next = &head;
            head.prev = &head;
        }

        size_type size() const {
            return count;
        }

        size_type capacity() const {
            return count;
        }

        bool empty() const {
            return !size();
        }

        iterator begin_iterator() { return iterator(head.next); }

        iterator end_iterator() { return iterator(&head); }

        void push_back(const T &o) {
//            list_node *node = newObject<list_node>(o, &head, head.prev);
            list_node *node = new list_node(o, &head, head.prev);
            head.prev->next = node;
            head.prev = node;
            ++count;
        }

        void push_back(T &&o) {
//            list_node *node = newObject<list_node>(forward<T>(o), &head, head.prev);
            list_node *node = new list_node(forward<T>(o), &head, head.prev);
            head.prev->next = node;
            head.prev = node;
            ++count;
        }

        void push_front(const T &o) {
//            list_node *node = newObject<list_node>(o, head.next, &head);
            list_node *node = new list_node(o, head.next, &head);
            head.next->prev = node;
            head.next = node;
            ++count;
        }

        void push_front(T &&o) {
//            list_node *node = newObject<list_node>(forward<T>(o), head.next, &head);
            list_node *node = new list_node(forward<T>(o), head.next, &head);
            head.next->prev = node;
            head.next = node;
            ++count;
        }

        T pop_front() {
            list_node *node = head.next;
            T obj{move(node->obj)};
            head.next = node->next;
            node->next->prev = &head;
            --count;
            delete (node);
            return move(obj);
        }

        T pop_back() {
            list_node *node = head.prev;
            T obj{move(node->obj)};
            head.prev = node->prev;
            node->prev->next = &head;
            delete (node);
            --count;
            return move(obj);
        }

        iterator remove(iterator it) {
            list_node *prev = it.node->prev, *next = it.node->next;
            prev->next = next;
            next->prev = prev;
            delete (it.node);
            --count;
            return iterator(next);  // the next one
        }

        iterator insert(iterator pos, const T &o) {
            insert_before(pos, o);
            return pos;
        }

        iterator insert(iterator pos, T &&o) {
            insert_before(pos, forward(o));
            return pos;
        }

        template <typename It>
        iterator insert(iterator pos, It begin, It end) {
            for (; begin != end; ++begin) {
                insert(pos, *begin);
            }
            return pos;
        }

        bool operator==(const LinkedList &rhs) const {
            if (&rhs == this) {
                return true;
            }
            if (rhs.size() != size()) {
                return false;
            }
            list_node *x = head.next, *y = rhs.head.next;
            while (x != &head) {
                if (*x->obj != *y->obj) {
                    return false;
                }
                x = x->next;
                y = y->next;
            }
            return true;
        }

        bool operator!=(const LinkedList &rhs) const {
            return !(rhs == *this);
        }

        iterator find(const T &o) const {
            list_node *node = head.next;
            while (node != &head) {
                if (node->obj == o) {
                    return iterator(node);
                }
                node = node->next;
            }
            return end_iterator();
        }
    };

    template <typename T>
    void swap(typename LinkedList<T>::iterator x, typename LinkedList<T>::iterator y) {
        LinkedList<T>::swap_nodes(x.operator->(), y.operator->());
    }

};
#endif //MYCPPLIB_LINKED_LIST_HPP
