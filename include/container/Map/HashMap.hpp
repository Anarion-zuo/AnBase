//
// Created by anarion on 2020/1/31.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_HASH_MAP_HPP
#define MYCPPLIB_HASH_MAP_HPP

#include <initializer_list>
#include "../Set/HashSet.hpp"
#include "MapEntry.hpp"

namespace anarion {

    template<typename K, typename V, typename hash_func = hash_function<MapEntry<K, V>>>
    class HashMap : public HashSet<MapEntry<K, V>, hash_func> {
        typedef HashSet<MapEntry<K, V>, hash_func> parent;
        typedef HashMap<K, V, hash_func> this_type;
        typedef MapEntry<K, V> entry;
        typedef typename parent::hash_node entry_node;

    protected:
        typename parent::hash_node *find_node(const K &o, hash_type hash_val) const {
            if (this->heads_count == 0) { return nullptr; }
            size_type index = hash_val % this->heads_count;
            typename parent::hash_node *node = this->heads[index];
            while (node) {
                if (node->obj == o) {
                    break;
                }
                node = node->next;
            }
            return node;
        }

    public:
        HashMap() = default;

        HashMap(std::initializer_list<entry> initList) {
            for (auto &it : initList) {
                this->insert(move(it));
            }
        }

        HashMap(const this_type &rhs) : parent(rhs) {}
        HashMap(this_type &&rhs) noexcept : parent((forward<this_type>(rhs))) {}

        this_type &operator=(const this_type &rhs) {
            if (&rhs == this) { return *this; }
            parent::operator=(rhs);
            return *this;
        }

        this_type &operator=(this_type &&rhs) noexcept {
            parent::operator=(forward<this_type>(rhs));
            return *this;
        }

        void put(const K &key, const V &val) { parent::insert(MapEntry<K, V>(key, val)); }
        void put(K &&key, const V &val) { parent::insert(MapEntry<K, V>(forward<K>(key), val)); }
        void put(const K &key, V &&val) { parent::insert(MapEntry<K, V>(key, forward<V>(val))); }
        void put(K &&key, V &&val) { parent::insert(MapEntry<K, V>(forward<K>(key), forward<V>(val))); }

        V& get(const K &key) const {
            hash_func func;
            return find_node(key, func(key))->obj.get_val();
        }

        bool has_key(const K &key) const {
            hash_func func;
            return find_node(key, func(key));
        }

        typename parent::iterator find(const K &key) const {
            if (this->heads_count == 0 || this->obj_count == 0) {
                return this->end_iterator();
            }
            hash_func func;
            hash_type hash_val = func(key);
            size_type index = hash_val % this->heads_count;
            entry_node *node = find_node(key, hash_val);
            if (node == nullptr) {
                return this->end_iterator();
            }
            return typename parent::iterator(node, index, this);
        }
    };
};

#endif //MYCPPLIB_HASH_MAP_HPP
