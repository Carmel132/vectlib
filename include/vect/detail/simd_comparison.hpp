#pragma once
#include "vect/detail/simd_mask.hpp"

namespace vect::detail
{
    struct ComparisonGreater
    {
        template <typename T>
        auto operator()(T a, T b) const { return a > b; }

        auto operator()(Packet4f a, Packet4f b) const -> Mask4f
        {
            return Mask4f::fromReg(_mm_cmpgt_ps(a.reg, b.reg));
        }

        auto operator()(Packet8f a, Packet8f b) const -> Mask8f
        {
            return Mask8f::fromReg(_mm256_cmp_ps(a.reg, b.reg, _CMP_GT_OQ));
        }

        auto operator()(Packet2d a, Packet2d b) const -> Mask2d
        {
            return Mask2d::fromReg(_mm_cmpgt_pd(a.reg, b.reg));
        }

        auto operator()(Packet4d a, Packet4d b) const -> Mask4d
        {
            return Mask4d::fromReg(_mm256_cmp_pd(a.reg, b.reg, _CMP_GT_OQ));
        }

        auto operator()(Packet4i a, Packet4i b) const -> Mask4i
        {
            return Mask4i::fromReg(_mm_cmpgt_epi32(a.reg, b.reg));
        }

        auto operator()(Packet8i a, Packet8i b) const -> Mask8i
        {
            return Mask8i::fromReg(_mm256_cmpgt_epi32(a.reg, b.reg));
        }
    };

    struct ComparisonLess
    {
        template <typename T>
        auto operator()(T a, T b) const { return a < b; }

        auto operator()(Packet4f a, Packet4f b) const -> Mask4f
        {
            return Mask4f::fromReg(_mm_cmplt_ps(a.reg, b.reg));
        }

        auto operator()(Packet8f a, Packet8f b) const -> Mask8f
        {
            return Mask8f::fromReg(_mm256_cmp_ps(a.reg, b.reg, _CMP_LT_OQ));
        }

        auto operator()(Packet2d a, Packet2d b) const -> Mask2d
        {
            return Mask2d::fromReg(_mm_cmplt_pd(a.reg, b.reg));
        }

        auto operator()(Packet4d a, Packet4d b) const -> Mask4d
        {
            return Mask4d::fromReg(_mm256_cmp_pd(a.reg, b.reg, _CMP_LT_OQ));
        }

        auto operator()(Packet4i a, Packet4i b) const -> Mask4i
        {
            return Mask4i::fromReg(_mm_cmpgt_epi32(b.reg, a.reg));
        }

        auto operator()(Packet8i a, Packet8i b) const -> Mask8i
        {
            return Mask8i::fromReg(_mm256_cmpgt_epi32(b.reg, a.reg));
        }
    };

    struct ComparisonGreaterEqual
    {
        template <typename T>
        auto operator()(T a, T b) const { return a >= b; }

        auto operator()(Packet4f a, Packet4f b) const -> Mask4f
        {
            return Mask4f::fromReg(_mm_cmpge_ps(a.reg, b.reg));
        }

        auto operator()(Packet8f a, Packet8f b) const -> Mask8f
        {
            return Mask8f::fromReg(_mm256_cmp_ps(a.reg, b.reg, _CMP_GE_OQ));
        }

        auto operator()(Packet2d a, Packet2d b) const -> Mask2d
        {
            return Mask2d::fromReg(_mm_cmpge_pd(a.reg, b.reg));
        }

        auto operator()(Packet4d a, Packet4d b) const -> Mask4d
        {
            return Mask4d::fromReg(_mm256_cmp_pd(a.reg, b.reg, _CMP_GE_OQ));
        }

        auto operator()(Packet4i a, Packet4i b) const -> Mask4i
        {
            __m128i ltMask = _mm_cmpgt_epi32(b.reg, a.reg);
            return Mask4i::fromReg(_mm_andnot_si128(ltMask, _mm_set1_epi32(-1)));
        }

        auto operator()(Packet8i a, Packet8i b) const -> Mask8i
        {
            __m256i ltMask = _mm256_cmpgt_epi32(b.reg, a.reg);
            return Mask8i::fromReg(_mm256_andnot_si256(ltMask, _mm256_set1_epi32(-1)));
        }
    };

    struct ComparisonLessEqual
    {
        template <typename T>
        auto operator()(T a, T b) const { return a <= b; }

        auto operator()(Packet4f a, Packet4f b) const -> Mask4f
        {
            return Mask4f::fromReg(_mm_cmple_ps(a.reg, b.reg));
        }

        auto operator()(Packet8f a, Packet8f b) const -> Mask8f
        {
            return Mask8f::fromReg(_mm256_cmp_ps(a.reg, b.reg, _CMP_LE_OQ));
        }

        auto operator()(Packet2d a, Packet2d b) const -> Mask2d
        {
            return Mask2d::fromReg(_mm_cmple_pd(a.reg, b.reg));
        }

        auto operator()(Packet4d a, Packet4d b) const -> Mask4d
        {
            return Mask4d::fromReg(_mm256_cmp_pd(a.reg, b.reg, _CMP_LE_OQ));
        }

        auto operator()(Packet4i a, Packet4i b) const -> Mask4i
        {
            __m128i gtMask = _mm_cmpgt_epi32(a.reg, b.reg);
            return Mask4i::fromReg(_mm_andnot_si128(gtMask, _mm_set1_epi32(-1)));
        }

        auto operator()(Packet8i a, Packet8i b) const -> Mask8i
        {
            __m256i gtMask = _mm256_cmpgt_epi32(a.reg, b.reg);
            return Mask8i::fromReg(_mm256_andnot_si256(gtMask, _mm256_set1_epi32(-1)));
        }
    };

    struct ComparisonEqual
    {
        template <typename T>
        auto operator()(T a, T b) const { return a == b; }

        auto operator()(Packet4f a, Packet4f b) const -> Mask4f
        {
            return Mask4f::fromReg(_mm_cmpeq_ps(a.reg, b.reg));
        }

        auto operator()(Packet8f a, Packet8f b) const -> Mask8f
        {
            return Mask8f::fromReg(_mm256_cmp_ps(a.reg, b.reg, _CMP_EQ_OQ));
        }

        auto operator()(Packet2d a, Packet2d b) const -> Mask2d
        {
            return Mask2d::fromReg(_mm_cmpeq_pd(a.reg, b.reg));
        }

        auto operator()(Packet4d a, Packet4d b) const -> Mask4d
        {
            return Mask4d::fromReg(_mm256_cmp_pd(a.reg, b.reg, _CMP_EQ_OQ));
        }

        auto operator()(Packet4i a, Packet4i b) const -> Mask4i
        {
            return Mask4i::fromReg(_mm_cmpeq_epi32(a.reg, b.reg));
        }

        auto operator()(Packet8i a, Packet8i b) const -> Mask8i
        {
            return Mask8i::fromReg(_mm256_cmpeq_epi32(a.reg, b.reg));
        }
    };

    struct ComparisonNotEqual
    {
        template <typename T>
        auto operator()(T a, T b) const { return a != b; }

        auto operator()(Packet4f a, Packet4f b) const -> Mask4f
        {
            return Mask4f::fromReg(_mm_cmpneq_ps(a.reg, b.reg));
        }

        auto operator()(Packet8f a, Packet8f b) const -> Mask8f
        {
            return Mask8f::fromReg(_mm256_cmp_ps(a.reg, b.reg, _CMP_NEQ_OQ));
        }

        auto operator()(Packet2d a, Packet2d b) const -> Mask2d
        {
            return Mask2d::fromReg(_mm_cmpneq_pd(a.reg, b.reg));
        }

        auto operator()(Packet4d a, Packet4d b) const -> Mask4d
        {
            return Mask4d::fromReg(_mm256_cmp_pd(a.reg, b.reg, _CMP_NEQ_OQ));
        }

        auto operator()(Packet4i a, Packet4i b) const -> Mask4i
        {
            __m128i eqMask = _mm_cmpeq_epi32(a.reg, b.reg);
            return Mask4i::fromReg(_mm_xor_si128(eqMask, _mm_set1_epi32(-1)));
        }

        auto operator()(Packet8i a, Packet8i b) const -> Mask8i
        {
            __m256i eqMask = _mm256_cmpeq_epi32(a.reg, b.reg);
            return Mask8i::fromReg(_mm256_xor_si256(eqMask, _mm256_set1_epi32(-1)));
        }
    };

    struct LogicalAnd
    {
        template <typename T>
        auto operator()(T a, T b) const { return a && b; }

        auto operator()(Mask4f a, Mask4f b) const -> Mask4f
        {
            return Mask4f::fromReg(_mm_and_ps(a.reg, b.reg));
        }

        auto operator()(Mask8f a, Mask8f b) const -> Mask8f
        {
            return Mask8f::fromReg(_mm256_and_ps(a.reg, b.reg));
        }

        auto operator()(Mask2d a, Mask2d b) const -> Mask2d
        {
            return Mask2d::fromReg(_mm_and_pd(a.reg, b.reg));
        }

        auto operator()(Mask4d a, Mask4d b) const -> Mask4d
        {
            return Mask4d::fromReg(_mm256_and_pd(a.reg, b.reg));
        }

        auto operator()(Mask4i a, Mask4i b) const -> Mask4i
        {
            return Mask4i::fromReg(_mm_and_si128(a.reg, b.reg));
        }

        auto operator()(Mask8i a, Mask8i b) const -> Mask8i
        {
            return Mask8i::fromReg(_mm256_and_si256(a.reg, b.reg));
        }
    };

    struct LogicalOr
    {
        template <typename T>
        auto operator()(T a, T b) const { return a || b; }

        auto operator()(Mask4f a, Mask4f b) const -> Mask4f
        {
            return Mask4f::fromReg(_mm_or_ps(a.reg, b.reg));
        }

        auto operator()(Mask8f a, Mask8f b) const -> Mask8f
        {
            return Mask8f::fromReg(_mm256_or_ps(a.reg, b.reg));
        }

        auto operator()(Mask2d a, Mask2d b) const -> Mask2d
        {
            return Mask2d::fromReg(_mm_or_pd(a.reg, b.reg));
        }

        auto operator()(Mask4d a, Mask4d b) const -> Mask4d
        {
            return Mask4d::fromReg(_mm256_or_pd(a.reg, b.reg));
        }

        auto operator()(Mask4i a, Mask4i b) const -> Mask4i
        {
            return Mask4i::fromReg(_mm_or_si128(a.reg, b.reg));
        }

        auto operator()(Mask8i a, Mask8i b) const -> Mask8i
        {
            return Mask8i::fromReg(_mm256_or_si256(a.reg, b.reg));
        }
    };

    struct LogicalNot
    {
        template <typename T>
        auto operator()(T a) const { return !a; }

        auto operator()(Mask4f a) const -> Mask4f
        {
            __m128 ones = _mm_castsi128_ps(_mm_set1_epi32(-1));
            return Mask4f::fromReg(_mm_xor_ps(a.reg, ones));
        }

        auto operator()(Mask8f a) const -> Mask8f
        {
            __m256 ones = _mm256_castsi256_ps(_mm256_set1_epi32(-1));
            return Mask8f::fromReg(_mm256_xor_ps(a.reg, ones));
        }

        auto operator()(Mask2d a) const -> Mask2d
        {
            __m128d ones = _mm_castsi128_pd(_mm_set1_epi32(-1));
            return Mask2d::fromReg(_mm_xor_pd(a.reg, ones));
        }

        auto operator()(Mask4d a) const -> Mask4d
        {
            __m256d ones = _mm256_castsi256_pd(_mm256_set1_epi32(-1));
            return Mask4d::fromReg(_mm256_xor_pd(a.reg, ones));
        }

        auto operator()(Mask4i a) const -> Mask4i
        {
            return Mask4i::fromReg(_mm_xor_si128(a.reg, _mm_set1_epi32(-1)));
        }

        auto operator()(Mask8i a) const -> Mask8i
        {
            return Mask8i::fromReg(_mm256_xor_si256(a.reg, _mm256_set1_epi32(-1)));
        }
    };

}
