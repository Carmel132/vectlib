#pragma once
#include "vect/core/vec_expr.hpp"
#include "vect/expr/capture_strategy.hpp"
#include "vect/detail/simd_packet.hpp"
namespace vect::expr
{
    template <typename L, typename R, typename Op>
    class BinaryOp : public core::VecExpr<BinaryOp<L, R, Op>>
    {
        core::capture_t<L> l_;
        core::capture_t<R> r_;
        Op op_;

    public:
        using valueType = std::common_type_t<typename L::valueType, typename R::valueType>;
        static constexpr size_t dim = L::dim;
        BinaryOp(const L &l, const R &r, Op op = Op{}) : l_{l}, r_{r}, op_{op} {}

        auto operator[](size_t idx) const -> valueType
        {
            return op_(l_[idx], r_[idx]);
        }

        [[nodiscard]] auto size() const -> size_t
        {
            return l_.size() != 0 ? l_.size() : r_.size();
        }

        auto loadPacket(size_t idx) const
        {
            auto leftPacket = l_.loadPacket(idx);
            auto rightPacket = r_.loadPacket(idx);

            return applyOp(leftPacket, rightPacket);
        }

        auto loadPacketUnaligned(size_t idx) const
        {
            auto leftPacket = l_.loadPacketUnaligned(idx);
            auto rightPacket = r_.loadPacketUnaligned(idx);

            return applyOp(leftPacket, rightPacket);
        }

        private:
        template <typename P>
        auto applyOp(const P& leftPacket, const P& rightPacket) const {
        
            if constexpr (requires {op_(leftPacket, rightPacket);}) {
                return op_(leftPacket, rightPacket);
            } else {
                using T = typename L::valueType;
                using Traits = detail::SimdTraits<T, L::dim>;
                constexpr size_t width = Traits::width;
                alignas(Traits::alignment) T lvals[width];
                alignas(Traits::alignment) T rvals[width];
                alignas(Traits::alignment) T res[width];

                leftPacket.store(lvals);
                rightPacket.store(rvals);

                for (size_t idx{}; idx < width; ++idx) {
                    res[idx] = op_(lvals[idx], rvals[idx]);
                }

                return P::load(res);
            }
        
        }
    };
}