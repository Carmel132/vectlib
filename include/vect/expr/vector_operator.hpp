#pragma once
#include "vect/expr/vector.hpp"
#include "vect/expr/binary_op.hpp"
#include <functional>
namespace vect::core {
    template <IsVecExpr L, IsVecExpr R>
    auto operator+(const L &l, const R &r)
    {
        return expr::BinaryOp<L, R, std::plus<>>(l, r);
    }

    template <IsVecExpr L, IsVecExpr R>
    auto operator-(const L& l, const R& r) {
        return expr::BinaryOp<L, R, std::minus<>>(l, r);
    }

    // Component-wise and scalar multiplication  
    template <IsVecExpr L, IsVecExpr R>
    auto operator*(const L& l, const R& r) {
        return expr::BinaryOp<L, R, std::multiplies<>>(l, r);
    }

    template <IsVecExpr V, Scalar S>
    auto operator*(const V& v, S s) {
        using ScalarExpr = VecScalar<S>;
        return expr::BinaryOp<V, ScalarExpr, std::multiplies<>>(v, ScalarExpr(s));
    }

    template <Scalar S, IsVecExpr V>
    auto operator*(S s, const V& v) {
        return v * s;
    }

    template <IsVecExpr L, IsVecExpr R>
    auto operator^(const L& l, const R& r) {
        using T = std::common_type_t<typename L::valueType, typename R::valueType>;
        T sum = 0;
        for (size_t idx{}; idx < l.size(); ++idx) {
            sum += l[idx] * r[idx];
        }
        return sum;
    }
}