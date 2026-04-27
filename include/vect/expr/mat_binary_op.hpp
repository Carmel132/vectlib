#pragma once
#include "vect/core/mat_expr.hpp"
#include "vect/expr/binary_op.hpp"
#include "vect/expr/capture_strategy.hpp"
namespace vect::expr
{

    /// @brief Expression node for component-wise binary operations on matrices
     /// @tparam L Left-hand side matrix expression type
     /// @tparam R Right-hand side matrix expression type
     /// @tparam Op Binary operation type
    template <typename L, typename R, typename Op>
    class MatBinaryOp : public core::MatExpr<MatBinaryOp<L, R, Op>, typename L::valueType, L::rows, L::columns>
    {

        core::capture_t<L> l_;
        core::capture_t<R> r_;
        Op op_;

    public:
        MatBinaryOp(const L &l, const R &r, Op op = Op{}) : l_{l}, r_{r}, op_{op} {}

        [[nodiscard]] auto at(size_t r, size_t c) const
        {
            return op_(l_(r, c), r_(r, c));
        }

        [[nodiscard]] auto getRow(size_t r) const
        {
            return op_(l_.getRow(r), r_.getRow(r));
        }
    };
}