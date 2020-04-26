//
// Created by anarion on 2020/2/2.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_HASH_SET_HPP
#define MYCPPLIB_HASH_SET_HPP

class iterator;

#include <cstring>
#include "../base/container_utility.hpp"
#include "allocator/Allocator.h"
#include "../base/iterator_traits.hpp"
#include <utility>

namespace anarion {

    typedef size_type hash_type;
    typedef hash_type (*hash_func_impl_type)(void *p, size_type len);

    template <typename T> struct hash_function {
        hash_type operator()(const T &o) { return static_cast<hash_type>(&o); }
    };
    template <typename T> struct hash_function<const T> { hash_type operator()(const T &o) { return hash_function<T>().operator()(o); } };
    template <typename T> struct hash_function<const T&> { hash_type operator()(const T &o) { return hash_function<T>().operator()(o); } };

    template<typename T> struct hash_function<T*> { hash_type operator()(T *o) { return reinterpret_cast<hash_type>(o); } };
    template<typename T> struct hash_function<const T*> { hash_type operator()(const T *o) { return reinterpret_cast<hash_type>(o); } };

    template <> struct hash_function<char> { hash_type operator()(unsigned long o) {return reinterpret_cast<hash_type>(o); } };
    template <> struct hash_function<short> { hash_type operator()(unsigned long o) {return reinterpret_cast<hash_type>(o); } };
    template <> struct hash_function<int> { hash_type operator()(unsigned long o) {return reinterpret_cast<hash_type>(o); } };
    template <> struct hash_function<long> { hash_type operator()(unsigned long o) {return reinterpret_cast<hash_type>(o); } };
    template <> struct hash_function<unsigned char> { hash_type operator()(unsigned long o) {return reinterpret_cast<hash_type>(o); } };
    template <> struct hash_function<unsigned short> { hash_type operator()(unsigned long o) {return reinterpret_cast<hash_type>(o); } };
    template <> struct hash_function<unsigned int> { hash_type operator()(unsigned long o) {return reinterpret_cast<hash_type>(o); } };
    template <> struct hash_function<unsigned long> { hash_type operator()(unsigned long o) {return reinterpret_cast<hash_type>(o); } };
    template <> struct hash_function<float> { hash_type operator()(float o) {return static_cast<hash_type>(o); } };
    template <> struct hash_function<double> { hash_type operator()(double o) {return static_cast<hash_type>(o); } };

    template<typename T, typename hash_func = hash_function<T>>
    class HashSet {
    public:
        struct hash_node {
            T obj;
            hash_type hash_val;
            hash_node *next;

            hash_node() : next(nullptr) {}
            hash_node(const T &o, hash_type hv, hash_node *next = nullptr) : obj(o), hash_val(hv), next(next) {}
            hash_node(T &&o, hash_type hv, hash_node *next = nullptr) : obj(forward<T>(o)), hash_val(hv), next(next) {}
            hash_node(const hash_node &rhs) : hash_val(rhs.hash_val), obj(rhs.o), next(nullptr) {}
            hash_node(hash_node &&rhs) noexcept : hash_val(rhs.hash_val), obj(move(rhs.obj)), next(nullptr) { rhs.next = nullptr; }
        };

    protected:
        hash_node **heads = nullptr;
        size_type heads_count = 0, obj_count = 0;

        void clear_move() {
            heads = nullptr;
            heads_count = 0;
            obj_count = 0;
        }

        // delete the rest of the list
        static void del_list(hash_node *node) {
            while (node) {
                hash_node *next = node->next;
                delete (node);
                node = next;
            }
        }

        void expand_heads() {
            if (heads_count == 0) {
                heads_count = 8;
                heads = (hash_node**)operator new(heads_count * sizeof(hash_node*));
                memset(heads, 0, sizeof(hash_node*) * heads_count);
                return;
            }
            obj_count = 0;  // compatible with insert_node
            size_type old_count = heads_count, new_count = old_count << 1u;
            heads_count = new_count;  // update heads count
            hash_node **old_heads = heads;   // record old heads
            heads = (hash_node**)operator new(new_count * sizeof(hash_node*));  // update new heads
            memset(heads, 0, sizeof(hash_node*) * new_count);
            for (size_type i = 0; i < old_count; ++i) {
                hash_node *node = old_heads[i];
                while (node) {
                    hash_node *next = node->next;
                    insert_node(node);
                    node = next;
                }
            }
            operator delete (old_heads, old_count * sizeof(hash_node*));  // release old resource
        }

        void insert_node(hash_node *node) {
            hash_type hash_val = node->hash_val;
            size_type index = hash_val % heads_count;
            hash_node *head = heads[index];
            while (head) {
                if (head->obj == node->obj) {
                    delete node;
                    return;
                }
                head = head->next;
            }
            hash_node *next = heads[index];
            node->next = next;
            heads[index] = node;
            ++obj_count;
        }

        hash_node *find_node(const T &o, hash_type hash_val) const {
            hash_func func;
            size_type index = hash_val % heads_count;
            hash_node *node = heads[index];
            while (node) {
                if (node->obj == o) {
                    break;
                }
                node = node->next;
            }
            return node;
        }

        // ugly for efficiency
        void remove_node(const T &o, hash_type hash_val) {
            size_type index = hash_val % heads_count;
            hash_node *head = heads[index];
            if (head == nullptr) {
                // no node
                return;
            }
            // special check
            if (head->obj == o) {
                --obj_count;
                delete (head);
                heads[index] = heads[index]->next;
            }
            while (head->next) {
                if (head->next->obj == o) {
                    --obj_count;
                    hash_node *next = head->next->next;
                    delete (head->next);
                    head->next = next;
                    return;
                }
                head = head->next;
            }
        }

        hash_node *duplicate_list(hash_node *node) {
            if (node == nullptr) { return nullptr; }
            hash_node *new_node = new hash_node(node->obj, node->hash_val), *ret = new_node;
            while (node->next) {
                hash_node *next = new hash_node(node->obj, node->hash_val);
                new_node->next = next;
                new_node = next;
                node = node->next;
            }
            return ret;
        }

    public:
        HashSet() = default; // : heads_count(0), obj_count(0), heads(nullptr) {}

        HashSet(std::initializer_list<T> &&initList) {
            for (auto &item : initList) {
                insert(move(item));
            }
        }

        HashSet(const HashSet<T, hash_func> &rhs)
        : obj_count(rhs.obj_count), heads_count(rhs.heads_count), heads((hash_node**)operator new(heads_count * sizeof(hash_node**))) {
            memset(heads, 0, heads_count * sizeof(hash_node*));
            for (size_type i = 0; i < heads_count; ++i) {
                heads[i] = duplicate_list(heads[i]);
            }
        }

        HashSet(HashSet<T, hash_func> &&rhs) noexcept : heads(rhs.heads), heads_count(rhs.heads_count), obj_count(rhs.obj_count) { rhs.clear_move(); }

        HashSet<T, hash_func> &operator=(const HashSet<T, hash_func> &rhs) {
            if (this == &rhs) { return *this; }
            clear();
            obj_count = rhs.obj_count;
            heads_count = rhs.heads_count;
            heads = (hash_node**)operator new(heads_count * sizeof(hash_node**));
            memset(heads, 0, heads_count * sizeof(hash_node*));
            for (size_type i = 0; i < heads_count; ++i) {
                heads[i] = duplicate_list(heads[i]);
            }
            return *this;
        }

        HashSet<T, hash_func> &operator=(HashSet<T, hash_func> &&rhs) noexcept {
            clear();
            heads = rhs.heads;
            heads_count = rhs.heads_count;
            obj_count = rhs.obj_count;
            rhs.clear_move();
            return *this;
        }

        void clear() {
            // delete every list
            for (size_type i = 0; i < heads_count; ++i) {
                del_list(heads[i]);
            }

            // delete head array
//            delObjects(heads, heads_count);
            operator delete (heads, heads_count * sizeof(hash_node*));

            // clear members
            clear_move();
        }

        virtual ~HashSet() { clear(); }

        void insert(const T &o) {
            if (heads_count <= obj_count) {
                expand_heads();
            }
            hash_func func;
            hash_type hash_val = func(o);
//            hash_node *node = newObject<hash_node>(o, hash_val);
            hash_node *node = new hash_node(o, hash_val);
            insert_node(node);
        }

        void insert(T &&o) {
            if (heads_count <= obj_count) {
                expand_heads();
            }
            hash_func func;
            hash_type hash_val = func(o);
//            hash_node *node = newObject<hash_node>(forward(o), hash_val);
            hash_node *node = new hash_node(forward<T>(o), hash_val);
            insert_node(node);
        }

        bool has(const T &o) const { return find_node(o, hash_func().operator()(o)); }
        size_type size() const { return obj_count; }
        bool empty() const { return obj_count == 0; }

        void remove(const T &o) {
            hash_func func;
            hash_type hash_val = func(o);
            remove_node(o, hash_val);
        }

        struct iterator {
            /*
             * end iterator to be null node
             */
            hash_node *cur_node;
            const HashSet<T> *set;
            size_type index;

            // traits
            typedef const_iterator category;
            typedef long dif_type;
            typedef T val_type;
            typedef T* ptr_type;
            typedef T& ref_type;
            typedef T&& rval_type;

            iterator(hash_node *node, size_type index, const HashSet<T> *set) : cur_node(node), index(index), set(set) {}

            T &operator*() const { return cur_node->obj; }
            T *operator->() const { return &cur_node->obj; }

            iterator &operator++() {
                iterator newit = set->get_iterator_next(*this);
                cur_node = newit.cur_node;
                index = newit.index;
                set = newit.set;
                return *this;
            }

            iterator operator++(int) {
                iterator result(*this);
                ++(*this);
                return result;
            }

            bool operator==(const iterator &rhs) const {
                return cur_node == rhs.cur_node &&
                       set == rhs.set;
            }

            bool operator!=(const iterator &rhs) const {
                return !(rhs == *this);
            }
        };

        iterator begin_iterator() const {
            if (heads_count == 0) {
                return iterator(nullptr, 0, this);  // end iterator
            }
            hash_node *node = heads[0];
            size_type index = 0;
            while (node == nullptr && index < heads_count) {
                ++index;
                node = heads[index];
            }
            if (index == heads_count && node == nullptr) { return iterator(nullptr, 0, this); }
            return iterator(node, index, this);
        }

    protected:
        iterator get_iterator_next(iterator &it) const {
            hash_node *node = it.cur_node->next;
            size_type index = it.index;
            while (node == nullptr && index < heads_count) {
                ++index;
                node = heads[index];
            }
            if (index == heads_count) {
                return iterator(nullptr, 0, this);
            }
            return iterator(node, index, this);
        }

    public:
        iterator end_iterator() const {
            return iterator(nullptr, 0, this);
        }

        iterator find(const T &o) const {
            hash_func func;
            hash_type hash_val = func(o);
            size_type index = hash_val % heads_count;
            hash_node *node = find_node(o, hash_val);
            if (node == nullptr) {
                return end_iterator();
            }
            return iterator(node, index, this);
        }
    };



};

#endif //MYCPPLIB_HASH_SET_HPP
