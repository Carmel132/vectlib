#pragma once
#include "vect/detail/simd_packet.hpp"

namespace vect::detail
{
    struct Mask4f
    {
        __m128 reg;

        static auto fromReg(__m128 r) -> Mask4f { return {r}; }
    };

    struct Mask8f
    {
        __m256 reg;

        static auto fromReg(__m256 r) -> Mask8f { return {r}; }
    };

    struct Mask2d
    {
        __m128d reg;

        static auto fromReg(__m128d r) -> Mask2d { return {r}; }
    };

    struct Mask4d
    {
        __m256d reg;

        static auto fromReg(__m256d r) -> Mask4d { return {r}; }
    };

    struct Mask4i
    {
        __m128i reg;

        static auto fromReg(__m128i r) -> Mask4i { return {r}; }
    };

    struct Mask8i
    {
        __m256i reg;

        static auto fromReg(__m256i r) -> Mask8i { return {r}; }
    };
}