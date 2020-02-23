//
// Created by anarion on 2020/2/20.
//

#ifndef MYCPPBASE_TUPLE_HPP
#define MYCPPBASE_TUPLE_HPP

#include <exceptions/container/IndexOutOfRange.h>
#include "../base/container_utility.hpp"
#include "../base/type_trait.hpp"

namespace anarion {

    template <typename ...Args>
    class Tuple;

    template <>
    class Tuple<> {
    protected:
        template <size_type index>
        int get() const {
//            static_assert(index < sizeof...(Args));
            throw IndexOutOfRange();
        }
    };

    template <typename firstArg, typename ...Args>
    class Tuple<firstArg, Args...> : public Tuple<Args...> {
    protected:

        /*
         * type traits machine for type deduction
         * to deduct the type of the nth of a packet
         * following by a declaration without implementation and recursive implementation
         */
        template <size_type index, typename ...As>
        struct index_deduct;

        template <size_type index, typename firstA, typename ...As>
        struct index_deduct<index, firstA, As...> {
            typedef typename index_deduct<index - 1, As...>::value_type value_type;
            typedef typename index_deduct<index - 1, As...>::tuple_type tuple_type;
        };

        template <typename firstA, typename ...As>
        struct index_deduct<0, firstA, As...> {
            typedef firstA value_type;
            typedef Tuple<firstA, As...> tuple_type;
        };

        template <size_type index>
        struct index_deduct<index> {
            typedef void value_type;
            typedef Tuple<> tuple_type;
        };

        /*
         * typedefs for convenience
         */
        typedef Tuple<firstArg, Args...> this_type;

        // actual field
        firstArg arg;

    public:

        typedef Tuple<Args...> parent_type;

        constexpr firstArg &get() { return arg; }
        constexpr parent_type &get_parent() { return *this; }

        template <typename ...Ts>
        static bool has_parent(Tuple<Ts...> &tuple) { return true; }

        explicit Tuple(firstArg first, Args ...args) : parent_type(args...), arg(first) {}

        Tuple(const this_type &rhs) : parent_type(rhs), arg(rhs.arg) {}

        Tuple(this_type &&rhs) noexcept : parent_type(forward<this_type>(rhs)), arg(move(rhs.arg)) {}

        ~Tuple() = default;

        this_type &operator=(const this_type &rhs) {
            if (&rhs == this) { return *this; }
            parent_type::operator=(rhs);
            arg = rhs.arg;
            return *this;
        }

        this_type &operator=(this_type &&rhs) noexcept {
            if (&rhs == this) { return *this; }
            parent_type::operator=(forward<this_type>(rhs));
            arg = move(rhs.arg);
            return *this;
        }

        template <size_type index>
        constexpr typename index_deduct<index, firstArg, Args...>::value_type get() {
            typedef typename index_deduct<index, firstArg, Args...>::tuple_type target_type;
            return target_type::arg;
        }
    };

    template <size_type index, typename ...Args>
    constexpr auto get_tuple(Tuple<Args...> &tuple) {
        return tuple.template get<index>();
    }

    template <typename ...Args>
    constexpr Tuple<Args...> make_tuple(Args ...args) {
        return Tuple<Args...>(args...);
    }
}

#endif //MYCPPBASE_TUPLE_HPP
