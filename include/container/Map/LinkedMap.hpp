//
// Created by anarion on 2020/2/2.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_LINKED_MAP_HPP
#define MYCPPLIB_LINKED_MAP_HPP

#include "MapEntry.hpp"
#include "container/List/LinkedList.hpp"

namespace anarion {
    template<typename K, typename V>
    class LinkedMap : public LinkedList<MapEntry<K, V>> {
        typedef LinkedList<MapEntry<K, V>> parent;
        typedef LinkedMap<K, V> this_type;
        typedef MapEntry<K, V> entry;
        typedef typename parent::list_node entry_node;
    public:

    };
}

#endif //MYCPPLIB_LINKED_MAP_HPP
