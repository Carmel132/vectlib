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

    template <typename T> struct isMask : std::false_type {};
    template <> struct isMask<Mask4f> : std::true_type {};
    template <> struct isMask<Mask8f> : std::true_type {};
    template <> struct isMask<Mask2d> : std::true_type {};
    template <> struct isMask<Mask4d> : std::true_type {};
    template <> struct isMask<Mask4i> : std::true_type {};
    template <> struct isMask<Mask8i> : std::true_type {};

    template <typename T> inline constexpr bool isMask_v = isMask<T>::value;
}