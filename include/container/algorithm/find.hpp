#ifndef FIND_HPP
#define FIND_HPP

#include "../base/iterator_traits.hpp"

namespace anarion {

template <typename Iterator>
Iterator find(const Iterator &begin, const Iterator &end) {
    find_impl(begin, end, typename iterator_traits<Iterator>::category());
}

template <typename Iterator>
Iterator find_impl(const Iterator &begin, const Iterator &end, const_iterator) {

}

template <typename Iterator>
Iterator find_impl(const Iterator &begin, const Iterator &end, seq_iterator) {

}

template <typename Iterator>
Iterator find_impl(const Iterator &begin, const Iterator &end, random_iterator) {

}

}

#endif // FIND_HPP