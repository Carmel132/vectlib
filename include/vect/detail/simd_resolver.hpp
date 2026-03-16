#pragma once
namespace vect::expr { // forward declaration :wilted_rose:
    template <typename V, size_t... Indices> class Swizzle;
    template <typename V, typename Op> class UnaryOp;
    template <typename L, typename R, typename Op> class BinaryOp;
    
}

namespace vect::core {
    template <typename T, size_t N> class VecScalar;
    template <typename T, size_t N> class Vector;

}


namespace vect::detail {
    template <typename T>
    struct SimdResolver {
        using valueType = T;
        static constexpr size_t dim = 1;
        static constexpr bool isAligned = false;
    };

    template <typename V, size_t... Is>
    struct SimdResolver<expr::Swizzle<V, Is...>> {
        using valueType = typename V::valueType;
        static constexpr size_t dim = sizeof...(Is);
        static constexpr bool isAligned = false;
    };

    template <typename T, size_t N>
    struct SimdResolver<core::Vector<T, N>> {
        using valueType = T;
        static constexpr size_t dim = N;
        static constexpr bool isAligned = true;
    };

    template <typename T, size_t N>
    struct SimdResolver<core::VecScalar<T, N>> {
        using valueType = T;
        static constexpr size_t dim = 0;
        static constexpr bool isAligned = true;
    };

    template <typename E, typename Op>
    struct SimdResolver<expr::UnaryOp<E, Op>> {
        using valueType = typename E::valueType;
        static constexpr size_t dim = E::dim;
        static constexpr bool isAligned = SimdResolver<E>::isAligned;
    };

    template <typename L, typename R, typename Op>
    struct SimdResolver<expr::BinaryOp<L, R, Op>> {
        using valueType = typename L::valueType;
        static constexpr size_t dim = L::dim;
        static constexpr bool isAligned = SimdResolver<L>::isAligned && SimdResolver<R>::isAligned;
    };

}