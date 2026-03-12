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

            if constexpr (requires {op_(leftPacket, rightPacket); }) {
                return op_(leftPacket, rightPacket);
            }

            alignas(16)float lvals[4];
            alignas(16)float rvals[4];
            detail::Packet4f result;
            _mm_store_ps(lvals, leftPacket.reg);
            _mm_store_ps(rvals, rightPacket.reg);
            for (int j = 0; j < 4; ++j) {
                result.reg[j] = op_(lvals[j], rvals[j]);
            }
            return result;
        }
    };
}