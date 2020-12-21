//
// Created by anarion on 2020/2/1.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_ITERATOR_TRAITS_HPP
#define MYCPPLIB_ITERATOR_TRAITS_HPP



namespace anarion {

struct const_iterator {
    // read only
};

struct seq_iterator : public const_iterator {
    // support fast ++ -- slow + -
};

struct random_iterator : public seq_iterator {
    // fast + -
};

template <typename I> struct iterator_trait {
    typedef typename I::category category;
    typedef typename I::dif_type dif_type;
    typedef typename I::val_type val_type;
    typedef typename I::ptr_type ptr_type;
    typedef typename I::ref_type ref_type;
    typedef typename I::rval_type rval_type;
};

template <typename T> struct iterator_trait<T*> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef T val_type;
    typedef T* ptr_type;
    typedef T& ref_type;
    typedef T&& rval_type;
};

template <typename T> struct iterator_trait<const T*> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef T val_type;
    typedef T* ptr_type;
    typedef T& ref_type;
    typedef T&& rval_type;
};

template <> struct iterator_trait<char> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef char val_type;
    typedef char *ptr_type;
    typedef char &ref_type;
    typedef char rval_type;
};

template <> struct iterator_trait<short> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef short val_type;
    typedef short *ptr_type;
    typedef short &ref_type;
    typedef short rval_type;
};

template <> struct iterator_trait<int> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef int val_type;
    typedef int *ptr_type;
    typedef int &ref_type;
    typedef int rval_type;
};

template <> struct iterator_trait<long> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef long val_type;
    typedef long *ptr_type;
    typedef long &ref_type;
    typedef long rval_type;
};

template <> struct iterator_trait<unsigned char> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef unsigned char val_type;
    typedef unsigned char *ptr_type;
    typedef unsigned char &ref_type;
    typedef unsigned char rval_type;
};

template <> struct iterator_trait<unsigned short> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef unsigned short val_type;
    typedef unsigned short *ptr_type;
    typedef unsigned short &ref_type;
    typedef unsigned short rval_type;
};

template <> struct iterator_trait<unsigned int> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef unsigned int val_type;
    typedef unsigned int *ptr_type;
    typedef unsigned int &ref_type;
    typedef unsigned int rval_type;
};

template <> struct iterator_trait<unsigned long> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef unsigned long val_type;
    typedef unsigned long *ptr_type;
    typedef unsigned long &ref_type;
    typedef unsigned long rval_type;
};

template <> struct iterator_trait<float> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef float val_type;
    typedef float *ptr_type;
    typedef float &ref_type;
    typedef float rval_type;
};

template <> struct iterator_trait<double> {
    typedef random_iterator category;
    typedef long dif_type;
    typedef double val_type;
    typedef double *ptr_type;
    typedef double &ref_type;
    typedef double rval_type;
};

};

#endif //MYCPPLIB_ITERATOR_TRAITS_HPP
