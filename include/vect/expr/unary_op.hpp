#pragma once
#include "vect/core/vec_expr.hpp"
#include "vect/core/vector.hpp"
#include "vect/expr/capture_strategy.hpp"

namespace vect::expr
{
    template <typename V, typename Op>
    class UnaryOp : public core::VecExpr<UnaryOp<V, Op>>
    {
        core::capture_t<V> v_;
        Op op_;

    public:
        using valueType = std::invoke_result_t<Op, typename V::valueType>;
        static constexpr size_t dim = V::dim;

        constexpr UnaryOp(const V &v, Op op = Op{}) : v_{v}, op_{op} {}
        constexpr auto operator[](size_t idx) const
        {
            return op_(v_[idx]);
        }
        [[nodiscard]] constexpr auto size() const -> size_t{ return v_.size(); }
    };
}