#pragma once
#include <concepts>
#include <vect/core/vec_expr.hpp>
#include <immintrin.h>
namespace vect::core {
    template <typename T>
    concept Scalar = std::is_arithmetic_v<T>;

    template <typename T>
    class VecScalar : public core::VecExpr<VecScalar<T>> {
        T val_;
    public:
        static constexpr size_t dim = 0; // size agnostic
        using valueType = T;

        explicit VecScalar(T v) : val_{v} {}
        auto operator[](size_t idx) const -> T {return val_;}
        [[nodiscard]] auto size() const -> size_t {return 0;}

        auto loadPacket(size_t idx) const {
            return _mm_set1_ps(val_);
        }
    };
}