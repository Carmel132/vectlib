#pragma once
#include <concepts>
#include <vect/core/vec_expr.hpp>

namespace vect::core {
    template <typename T>
    concept Scalar = std::is_arithmetic_v<T>;

    template <typename T>
    class VecScalar : public core::VecExpr<VecScalar<T>> {
        T val_;
    public:
        using valueType = T;

        explicit VecScalar(T v) : val_{v} {}
        auto operator[](size_t idx) const -> T {return val_;}
        [[nodiscard]] auto size() const -> size_t {return 0;}
    };
}