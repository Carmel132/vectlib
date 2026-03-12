#pragma once
#include "vect/expr/binary_op.hpp"
#include "vect/core/scalar.hpp"
#include "vect/expr/vector_method.hpp"
#include "vect/detail/vector_method.hpp"
#include <functional>
#include <iostream>

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

    template <IsVecExpr V>
    constexpr auto operator-(const V& v) {
        return expr::negate(v);
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

    template <IsVecExpr V, Scalar S>
    auto operator/(const V& v, S s) {
        using ScalarExpr = VecScalar<S>;
        return expr::BinaryOp<V, ScalarExpr, std::divides<>>(v, ScalarExpr(s));
    }

    template <IsVecExpr V>
    std::ostream& operator<<(std::ostream& os, const V& v) {
        os << "[";
        for (size_t idx{}; idx < v.size(); ++idx) {
            os << v[idx] << (idx == v.size() - 1 ? "" : "  ");
        }
        return os << "]";
    }    

    template <IsVecExpr L, IsVecExpr R>
    auto operator==(const L& l, const R& r) {
        if (l.size() != r.size()) {
            return false;
        }

        return detail::all_equal_fold_impl(l, r, std::make_index_sequence<L::dim>{});
    }

    template <IsVecExpr L, IsVecExpr R>
    auto operator!=(const L& l, const R& r) {
        return !(l == r);
    }
}