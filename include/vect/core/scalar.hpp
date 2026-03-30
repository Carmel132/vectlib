#pragma once
#include "vect/detail/simd_traits.hpp"
#include <concepts>
#include <vect/core/vec_expr.hpp>
namespace vect::core
{
    template <typename T>
    concept Scalar = std::is_arithmetic_v<T>;

    template <typename T, size_t N>
    class VecScalar : public core::VecExpr<VecScalar<T, N>>
    {
        T val_;

    public:
        static constexpr size_t dim = N; // size agnostic
        using valueType = T;

        template <typename U> requires std::is_arithmetic_v<U> explicit VecScalar(U v) : val_{static_cast<T>(v)} {}
        auto operator[](size_t idx) const -> T { return val_; }
        [[nodiscard]] auto size() const -> size_t { return 0; }

        [[nodiscard]] auto loadPacket(size_t idx) const
        {
            using Traits = detail::SimdTraits<T, N>;
            using Packet = typename Traits::packetType;
            return Packet::broadcast(val_);
        }

        [[nodiscard]] auto loadPacketUnaligned(size_t idx) const
        {
            return loadPacket(idx);
        }
    };
}