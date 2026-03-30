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

    template <typename T> struct is_mask : std::false_type {};
    template <> struct is_mask<Mask4f> : std::true_type {};
    template <> struct is_mask<Mask8f> : std::true_type {};
    template <> struct is_mask<Mask2d> : std::true_type {};
    template <> struct is_mask<Mask4d> : std::true_type {};
    template <> struct is_mask<Mask4i> : std::true_type {};
    template <> struct is_mask<Mask8i> : std::true_type {};

    template <typename T> inline constexpr bool is_mask_v = is_mask<T>::value;
}