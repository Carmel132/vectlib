#pragma once
#include "vect/core/mat_expr.hpp"
#include "vect/expr/capture_strategy.hpp"
namespace vect::expr {

    template <typename M, typename Op>
    class MatUnaryOp : public core::MatExpr<MatUnaryOp<M, Op>, typename M::valueType, M::rows, M::columns>
    {
        core::capture_t<M> m_;
        Op op_;
    public:
        MatUnaryOp(const M& m, Op op = Op{}) : m_{m}, op_{op} {}

        [[nodiscard]] auto at(size_t r, size_t c) const {
            return op_(m_(r, c));
        }

        [[nodiscard]] auto getRow(size_t r) const {
            return op_(m_.getRow(r));
        }
    };
}
