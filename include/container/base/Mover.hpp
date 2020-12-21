//
// Created by anarion on 8/6/20.
//

#ifndef ANBASE_MOVER_HPP
#define ANBASE_MOVER_HPP

#include "Copier.hpp"

namespace anarion {



    template <typename movedType>
    class Mover {
        using has_move_ctor = typename move_traits<movedType>::has_move_cotr;
    public:
        void move(movedType *dst, const movedType *src, size_type num) {
            moveImpl(dst, src, num, has_move_ctor());
        }

    protected:
        void moveImpl(movedType *dst, const movedType *src, size_type num, true_type) {

        }
        void moveImpl(movedType *dst, const movedType *src, size_type num, true_type)
    };
}

#endif //ANBASE_MOVER_HPP
