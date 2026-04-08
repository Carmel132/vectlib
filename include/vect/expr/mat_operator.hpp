#pragma once

#include "vect/core/mat_expr.hpp"
#include "vect/expr/mat_binary_op.hpp"
#include "vect/expr/mat_unary_op.hpp"
#include <functional>
namespace vect::core
{

    template <IsMatExpr L, IsMatExpr R>
    auto operator+(const L &l, const R &r)
    {
        return expr::MatBinaryOp<L, R, std::plus<>>(l, r);
    }

    template <IsMatExpr L, IsMatExpr R>
    auto operator-(const L &l, const R &r)
    {
        return expr::MatBinaryOp<L, R, std::minus<>>(l, r);
    }

    template <IsMatExpr M>
    auto operator-(const M& m) {
        return expr::MatUnaryOp<M, std::negate<>>(m);
    }
}