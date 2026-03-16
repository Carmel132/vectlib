#pragma once
namespace vect::expr { // forward declaration :wilted_rose:
    template <typename V, size_t... Indices> class Swizzle;
}


namespace vect::detail {
    template <typename T>
    struct SimdResolver {
        using valueType = T;
        static constexpr size_t dim = 1;
    };

    template <typename V, size_t... Is>
    struct SimdResolver<expr::Swizzle<V, Is...>> {
        using valueType = typename V::valueType;
        static constexpr size_t dim = sizeof...(Is);
    };

}