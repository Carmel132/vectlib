#pragma once
#include <type_traits>

namespace vect::core {
    template <typename T>
    struct IsLeaf : std::false_type {};

    template <typename T, size_t N>
    struct IsLeaf<Vector<T, N>> : std::true_type {};

    template <typename T>
    struct CaptureStrategy {
        static constexpr bool useRef = IsLeaf<T>::value;
        using type = std::conditional_t<useRef, const T&, const T>;
    };

    template <typename T>
    using capture_t = typename CaptureStrategy<T>::type;
}