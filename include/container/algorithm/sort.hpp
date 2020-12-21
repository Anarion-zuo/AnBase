#ifndef SORT_HPP
#define SORT_HPP

#include "../base/iterator_traits.hpp"
#include "../../exceptions/InvalidOperation.h"

namespace anarion {

template <typename Iterator>
void sort(const Iterator &begin, const Iterator &end) {
    sort_impl(begin, end, typename iterator_traits<Iterator>::category());
}

template <typename Iterator>
void sort(const Iterator &begin, const Iterator &end, const_iterator) {
    throw InvalidOperation();
}

template <typename Iterator>
void sort(const Iterator &begin, const Iterator &end, seq_iterator) {
    insert_sort(begin, end);
}

template <typename Iterator>
void sort(const Iterator &begin, const Iterator &end, random_iterator) {
    
}

template <typename Iterator>
void insert_sort(const Iterator &begin, const Iterator &end) {
    
}

template <typename Iterator>
void quick_sort(const Iterator &begin, const Iterator &end) {

}

}

#endif // SORT_HPP