#pragma once

#include "vect/core/mat_expr.hpp"

namespace vect::core
{

    template <size_t R, size_t C, typename T, IsMatExpr<R, C, T> Left, IsMatExpr<R, C, T> Right>
    auto operator+(const Left &l, const Right &r)
    {
        return expr::MatBinaryOp<Left, Right, std::plus<>>(l, r);
    }
}