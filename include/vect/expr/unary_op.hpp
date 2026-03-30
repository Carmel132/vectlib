#pragma once
#include "vect/core/vec_expr.hpp"
#include "vect/core/vector.hpp"
#include "vect/detail/simd_packet.hpp"
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
        [[nodiscard]] constexpr auto size() const -> size_t { return v_.size(); }

        [[nodiscard]] auto loadPacket(size_t idx) const
        {
            return applyOp(v_.loadPacket(idx));
        }

        [[nodiscard]] auto loadPacketUnaligned(size_t idx) const
        {
            return applyOp(v_.loadPacketUnaligned(idx));
        }

    private:
        template <typename P>
        [[nodiscard]] auto applyOp(const P &packet) const
        {
            if constexpr (requires { op_(packet); })
            {
                return op_(packet);
            }
            else
            {
                using T = typename V::valueType;
                using Traits = detail::SimdTraits<T, V::dim>;
                constexpr size_t width = Traits::width;
                alignas(Traits::alignment) T vals[width];
                alignas(Traits::alignment) T res[width];

                packet.store(vals);

                for (size_t j = 0; j < width; ++j)
                {
                    res[j] = op_(vals[j]);
                }

                return P::load(res);
            }
        }
    };
}