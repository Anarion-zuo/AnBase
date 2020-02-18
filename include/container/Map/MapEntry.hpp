//
// Created by anarion on 2020/2/2.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_MAP_ENTRY_HPP
#define MYCPPLIB_MAP_ENTRY_HPP

namespace anarion {
template<typename K, typename V>
class MapEntry {
    K key;
    V val;
public:
    MapEntry(const K &key, const V &val) : key(key), val(val) {}
    MapEntry(K &&key, V &&val) : key(forward<K>(key)), val(forward<V>(val)) {}
    MapEntry(const K &key, V &&val) : key(key), val(forward<V>(val)) {}
    MapEntry(K &&key, const V &val) : key(forward<K>(key)), val(val) {}
    MapEntry(const MapEntry &rhs) : key(rhs.key), val(rhs.val) {}
    MapEntry(MapEntry &&rhs) noexcept : key(move(rhs.key)), val(move(rhs.val)) {}

    const K &get_key() const { return key; }
    V &get_val() { return val; }

    bool operator==(const MapEntry &rhs) const {
        return key == rhs.key;
    }

    bool operator!=(const MapEntry &rhs) const {
        return !(rhs == *this);
    }

    bool operator==(const K &rhs) const {
        return key == rhs;
    }

    bool operator!=(const K &rhs) const {
        return !(rhs == *this);
    }
};

template <typename K, typename V>
struct hash_function<MapEntry<K, V>> {
    hash_type operator()(const MapEntry<K, V> &o) const { return hash_function().operator()(o.get_key()); }
    hash_type operator()(const K &o) const { return hash_function<K>().operator()(o); }
};
}


#endif //MYCPPLIB_MAP_ENTRY_HPP
