#pragma once
#include <cstddef>
#include <type_traits>
#include "vect/core/vector.hpp"

namespace vect::core {
template <typename T, size_t R, size_t C>
class Matrix;    

    template <typename T>
    struct IsLeaf : std::false_type {};

    template <typename T, size_t N>
    struct IsLeaf<Vector<T, N>> : std::true_type {};

    template <typename T, size_t R, size_t C>
    struct IsLeaf<Matrix<T, R, C>> : std::true_type {};

    template <typename T>
    struct CaptureStrategy {
        static constexpr bool useRef = IsLeaf<T>::value;
        using type = std::conditional_t<useRef, const T&, const T>;

    };

    template <typename T>
    using capture_t = typename CaptureStrategy<T>::type;
}