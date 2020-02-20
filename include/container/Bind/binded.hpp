//
// Created by anarion on 2020/2/20.
//

#ifndef MYCPPBASE_BINDED_HPP
#define MYCPPBASE_BINDED_HPP

#include "Tuple.hpp"

#define CALL_TUPLE(t)  t.get(), CALL_TUPLE(t.get_parent())

namespace anarion {
    template <typename T, typename Fn, typename ...Args>
    class binded {
    protected:

        template <size_type ...> struct index_tuple {};

        template <size_type num, size_type ...indexes>
        struct make_indexes : make_indexes<num - 1, num - 1, indexes...> {};

        template <size_type ...indexes>
        struct make_indexes<0, indexes...> {
            typedef index_tuple<indexes...> value_type;
        };

        typedef typename make_indexes<sizeof...(Args)>::value_type indexes;

        template <size_type ...indexes>
        constexpr auto invoke(index_tuple<indexes...>) {
            return (obj->*func)(get_tuple<indexes>(tuple)...);
        }

        // function type defined for convenience
        typedef Tuple<Args...> tuple_type;

        tuple_type tuple;
        Fn func;  // function pointer
        T *obj = nullptr;

    public:
        binded(Fn &&func, T *obj, Args &&...args) : func(forward<Fn>(func)), obj(obj), tuple(forward<Args>(args)...) {}
        binded(const binded &) = default;
        binded(binded &&) noexcept = default;

        constexpr auto operator()() { return invoke(indexes()); }
    };

    template <typename Fn, typename ...Args>
    class binded<void, Fn, Args...> {
    protected:

        template <size_type ...> struct index_tuple {};

        template <size_type num, size_type ...indexes>
        struct make_indexes : make_indexes<num - 1, num - 1, indexes...> {};

        template <size_type ...indexes>
        struct make_indexes<0, indexes...> {
            typedef index_tuple<indexes...> value_type;
        };

        typedef typename make_indexes<sizeof...(Args)>::value_type indexes;

        template <size_type ...indexes>
        constexpr auto invoke(index_tuple<indexes...>) {
            return func(get_tuple<indexes>(tuple)...);
        }

        // function type defined for convenience
        typedef Tuple<Args...> tuple_type;

        tuple_type tuple;
        Fn func;  // function pointer

    public:
        binded(Fn &&func, Args &&...args) : func(forward<Fn>(func)), tuple(forward<Args>(args)...) {}
        binded(const binded &) = default;
        binded(binded &&) noexcept = default;

        constexpr auto operator()() { return invoke(indexes()); }
    };

//    template <typename Fn, typename ...Args>
//    class binded_bare {
//    protected:
//
//        template <size_type ...> struct index_tuple {};
//
//        template <size_type num, size_type ...indexes>
//        struct make_indexes : make_indexes<num - 1, num - 1, indexes...> {};
//
//        template <size_type ...indexes>
//        struct make_indexes<0, indexes...> {
//            typedef index_tuple<indexes...> value_type;
//        };
//
//        typedef typename make_indexes<sizeof...(Args)>::value_type indexes;
//
//        template <size_type ...indexes>
//        constexpr auto invoke(index_tuple<indexes...>) {
//            return func(get_tuple<indexes>(tuple)...);
//        }
//
//        // function type defined for convenience
//        typedef Tuple<Args...> tuple_type;
//
//        tuple_type tuple;
//        Fn func;  // function pointer
//
//    public:
//        binded_bare(Fn &&func, Args &&...args) : func(forward<Fn>(func)), tuple(forward<Args>(args)...) {}
//        binded_bare(const binded_bare &) = default;
//        binded_bare(binded_bare &&) noexcept = default;
//
//        constexpr auto operator()() { return invoke(indexes()); }
//    };

//    template <typename Fn, typename ...Args>
//    constexpr binded<Fn, Args...> bind(Fn &&fn, Args &&...args) {
//        return binded<Fn, Args...>(forward<Fn>(fn), forward<Args>(args)...);
//    }

    template <typename T, typename Fn, typename ...Args>
    constexpr binded<T, Fn, Args...> bind(Fn &&fn, T &obj, Args &&...args) {
        return binded<T, Fn, Args...>(forward<Fn>(fn), &obj, forward<Args>(args)...);
    }

    template <typename Fn, typename ...Args>
    constexpr binded<void, Fn, Args...> bind(Fn &&fn, Args &&...args) {
        return binded<void, Fn, Args...>(forward<Fn>(fn), forward<Args>(args)...);
    }
}

#endif //MYCPPBASE_BINDED_HPP
