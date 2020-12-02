//
// Created by anarion on 8/6/20.
//

#ifndef ANBASE_COPIER_HPP
#define ANBASE_COPIER_HPP

#include <cstring>
#include <new>

#include "type_trait.hpp"
#include "container_utility.hpp"

namespace anarion {

    template <typename T>
    struct pod_traits {
        using is_pod = false_type;
    };

    template <typename T>
    struct pod_traits<T *> {
        using is_pod = true_type;
    };
    template <typename T>
    struct pod_traits<const T *> {
        using is_pod = true_type;
    };
    template <typename T>
    struct pod_traits<const T> {
        using is_pod = typename pod_traits<T>::is_pod;
    };

#pragma region basic_type_pod
    template <> struct pod_traits<char> { using is_pod = true_type; };
    template <> struct pod_traits<short> { using is_pod = true_type; };
    template <> struct pod_traits<int> { using is_pod = true_type; };
    template <> struct pod_traits<long> { using is_pod = true_type; };
    template <> struct pod_traits<long long> { using is_pod = true_type; };
    template <> struct pod_traits<unsigned char> { using is_pod = true_type; };
    template <> struct pod_traits<unsigned short> { using is_pod = true_type; };
    template <> struct pod_traits<unsigned int> { using is_pod = true_type; };
    template <> struct pod_traits<unsigned long> { using is_pod = true_type; };
    template <> struct pod_traits<unsigned long long> { using is_pod = true_type; };
    template <> struct pod_traits<float> { using is_pod = true_type; };
    template <> struct pod_traits<double> { using is_pod = true_type; };
    template <> struct pod_traits<long double> { using is_pod = true_type; };
#pragma endregion

    template <typename T>
    struct move_traits {
        using has_move_cotr = false_type;
    };

    template <typename T>
    struct move_traits<T *> {
        using has_move_ctor = false_type;
    };
    template <typename T>
    struct move_traits<const T *> {
        using has_move_ctor = false_type;
    };
    template <typename T>
    struct move_traits<const T> {
        using has_move_ctor = typename move_traits<T>::has_move_cotr;
    };

#pragma region basic_type_move
    template <> struct move_traits<char> { using has_move_ctor = false_type; };
    template <> struct move_traits<short> { using has_move_ctor = false_type; };
    template <> struct move_traits<int> { using has_move_ctor = false_type; };
    template <> struct move_traits<long> { using has_move_ctor = false_type; };
    template <> struct move_traits<long long> { using has_move_ctor = false_type; };
    template <> struct move_traits<unsigned char> { using has_move_ctor = false_type; };
    template <> struct move_traits<unsigned short> { using has_move_ctor = false_type; };
    template <> struct move_traits<unsigned int> { using has_move_ctor = false_type; };
    template <> struct move_traits<unsigned long> { using has_move_ctor = false_type; };
    template <> struct move_traits<unsigned long long> { using has_move_ctor = false_type; };
    template <> struct move_traits<float> { using has_move_ctor = false_type; };
    template <> struct move_traits<double> { using has_move_ctor = false_type; };
    template <> struct move_traits<long double> { using has_move_ctor = false_type; };
#pragma endregion

    using size_type = unsigned long;

    struct CopierException : public std::exception {};
    struct CopierOutOfRange : public CopierException {};

    template <typename copiedType>
    class Copier {
        using is_pod = typename pod_traits<copiedType>::is_pod;
        using has_move_ctor = typename move_traits<copiedType>::has_move_ctor;

    public:
        void copy(copiedType *dst, const copiedType *src, size_type num) {
            copyImpl(dst, src, num, is_pod());
        }
        void move(copiedType *dst, copiedType *src, size_type num) {
            moveImpl(dst, src, num, has_move_ctor());
        }
        void clearSpace(copiedType *begin, size_type length) {
            clearSpaceImpl(begin, length, is_pod());
        }
        void moveOverLap(copiedType *dst, copiedType *src, size_type srcLength) {
            if (src > dst) {
                if (src >= dst + srcLength) {
                    move(dst, src, srcLength);
                    return;
                }
                size_type begin = 0, end = src - dst, unoverlappedLength = end, overlappedLength = srcLength - unoverlappedLength;
                // move overlapped region first
                size_type length = end;
                while (true) {
                    if (end >= srcLength) {
                        length = srcLength - begin;
                        clearSpace(dst + begin, length);
                        move(dst + begin, src + begin, length);
                        break;
                    }
                    clearSpace(dst + begin, length);
                    move(dst + begin, src + begin, length);
                    begin = end;
                    end = begin + length;
                }
                // the rest unoverlapped
                clearSpace(dst + overlappedLength, unoverlappedLength);
                move(dst + overlappedLength, src + overlappedLength, unoverlappedLength);
            } else if (src <= dst) {
                if (src + srcLength < dst) {
                    move(dst, src, srcLength);
                    return;
                }
                size_type length = dst - src, begin = srcLength - length, end = srcLength, unoverlappedLength = length, overlappedLength = srcLength - unoverlappedLength;
                // move overlapped region first
                while (true) {
                    if (begin <= unoverlappedLength) {
                        begin = unoverlappedLength;
                        length = end - begin;
                        clearSpace(dst + begin, length);
                        move(dst + begin, src + begin, length);
                        break;
                    }
                    clearSpace(dst + begin, length);
                    move(dst + begin, src + begin, length);
                    end = begin;
                    begin = end - length;
                }
                // the rest unoverlapped
                clearSpace(dst, unoverlappedLength);
                move(dst, src, unoverlappedLength);
            } else {
                // src == dst
                return;
            }
        }

        /**
         * @brief Moves objects inside an array forwards within the same array.
         * @param p head pointer of the array.
         * @param length length of the array.
         * @param begin begin index to be moved forwards.
         * @param steps objective displacement.
         *
         * @details This method is used as a mean of shrinking address space. After this calling, the array would find spare raw space at the end of itself.
         */
        void moveForward(copiedType *p, size_type length, size_type begin, size_type steps) {
            if (length == 0) {
                return;
            }
            if (steps == 0) {
                return;
            }
            length -= begin;
            copiedType *dst = p + begin - steps, *src = p + begin;
            size_type times = length / steps, more = length % steps;
            for (size_type time = 0; time < times; ++time) {
                clearSpace(dst, steps);
                move(dst, src, steps);
                dst += steps;
                src += steps;
            }
            clearSpace(dst, more);
            move(dst, src, more);
            clearSpace(dst + more, steps);
        }
        /**
         * @brief Moves objects inside an array backwards within the same array. The array must have spare raw space at the end.
         * @param p head pointer of the array.
         * @param length length of the array.
         * @param begin begin index to be moved forwards.
         * @param steps objective displacement.
         *
         * @details This method is used as a mean of moving the spare raw space at the end of a continuous address space to the middle of it.
         */
        void moveBackward(copiedType *p, size_type length, size_type begin, size_type steps) {
            if (length == 0) {
                return;
            }
            length -= begin;
            copiedType *dst = p + length, *src = p + length - steps;
            move(dst, src, steps);
            dst -= steps;
            src -= steps;
            size_type times = length / steps, more = length % steps;
            for (size_type time = 0; time < times - 1; ++time) {
                clearSpace(dst, steps);
                move(dst, src, steps);
                dst -= steps;
                src -= steps;
            }
            dst += steps - more;
            src += steps - more;
            clearSpace(dst, more);
            move(dst, src, more);
            clearSpace(src, steps);
        }

    protected:
        void copyImpl(copiedType *dst, const copiedType *src, size_type num, true_type) {
            memcpy(dst, src, num * sizeof(copiedType));
        }
        void copyImpl(copiedType *dst, const copiedType *src, size_type num, false_type) {
            for (size_type index = 0; index < num; ++index) {
                new (&dst[index]) copiedType(src[index]);
            }
        }

        void moveImpl(copiedType *dst, copiedType *src, size_type num, true_type) {
            for (size_type index = 0; index < num; ++index) {
                new (&dst[index]) copiedType(anarion::move(src[index]));
            }
        }
        void moveImpl(copiedType *dst, copiedType *src, size_type num, false_type) {
            copyImpl(dst, src, num, is_pod());
        }
        void clearSpaceImpl(copiedType *begin, size_type length, true_type) {
            // nothing here
        }
        // call destructors
        void clearSpaceImpl(copiedType *begin, size_type length, false_type) {
            for (size_type index = 0; index < length; ++index) {
                begin[index].~copiedType();
            }
        }
    };
}

#endif //ANBASE_COPIER_HPP
