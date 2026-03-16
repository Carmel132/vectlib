#pragma once
#include "vect/detail/simd_traits.hpp"
#include "vect/expr/swizzle.hpp"

namespace vect::detail
{
    template <size_t... Is>
    inline auto simdSwizzle(Packet4f packet) -> Packet4f
    {
        static_assert(sizeof...(Is) == 4, "SSE Swizzle requires exactly 4 indices");

        constexpr size_t indices[] = {Is...};

        return { _mm_shuffle_ps(packet.reg, packet.reg, _MM_SHUFFLE(indices[3], indices[2], indices[1], indices[0])) };
    }

    template <size_t... Is>
    inline auto simdSwizzle(Packet8f packet) -> Packet8f
    {
        static_assert(sizeof...(Is) == 8, "AVX Swizzle requires exactly 8 indices");

        constexpr size_t indices[] = {Is...};
        alignas(32) float vals[8];
        packet.store(vals);

        alignas(32) float out[8];
        for (size_t i = 0; i < 8; ++i)
        {
            out[i] = vals[indices[i]];
        }

        return Packet8f{_mm256_load_ps(out)};
    }

    template <size_t... Is>
    inline auto simdSwizzle(Packet2d packet) -> Packet2d
    {
        static_assert(sizeof...(Is) == 2, "SSE2 Swizzle requires exactly 2 indices");

        constexpr size_t indices[] = {Is...};
        alignas(16) double vals[2];
        packet.store(vals);

        alignas(16) double out[2];
        for (size_t i = 0; i < 2; ++i)
        {
            out[i] = vals[indices[i]];
        }

        return {_mm_load_pd(out)};
    }

    template <size_t... Is>
    inline auto simdSwizzle(Packet4d packet) -> Packet4d
    {
        static_assert(sizeof...(Is) == 4, "AVX Swizzle requires exactly 4 indices");

        constexpr size_t indices[] = {Is...};
        alignas(32) double vals[4];
        packet.store(vals);

        alignas(32) double out[4];
        for (size_t i = 0; i < 4; ++i)
        {
            out[i] = vals[indices[i]];
        }

        return {_mm256_load_pd(out)};
    }

    template <size_t... Is>
    inline auto simdSwizzle(Packet4i packet) -> Packet4i
    {
        static_assert(sizeof...(Is) == 4, "SSE2 Swizzle requires exactly 4 indices");

        constexpr size_t indices[] = {Is...};
        alignas(16) int vals[4];
        packet.store(vals);

        alignas(16) int out[4];
        for (size_t i = 0; i < 4; ++i)
        {
            out[i] = vals[indices[i]];
        }

        return {_mm_load_si128((__m128i *)out)};
    }

    template <size_t... Is>
    inline auto simdSwizzle(Packet8i packet) -> Packet8i
    {
        static_assert(sizeof...(Is) == 8, "AVX Swizzle requires exactly 8 indices");

        constexpr size_t indices[] = {Is...};
        alignas(32) int vals[8];
        packet.store(vals);

        alignas(32) int out[8];
        for (size_t i = 0; i < 8; ++i)
        {
            out[i] = vals[indices[i]];
        }

        return {_mm256_load_si256((__m256i *)out)};
    }
}
