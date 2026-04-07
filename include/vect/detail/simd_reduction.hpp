#pragma once

#include "vect/detail/simd_mask.hpp"
#include "vect/detail/simd_packet.hpp"

namespace vect::detail
{

    inline Mask4f simdIsNonzero(Packet4f p)
    {
        return Mask4f(_mm_cmpneq_ps(p.reg, _mm_setzero_ps()));
    }

    inline Mask8f simdIsNonzero(Packet8f p)
    {
        return Mask8f(_mm256_cmp_ps(p.reg, _mm256_setzero_ps(), _CMP_NEQ_OQ));
    }

    inline Mask2d simdIsNonzero(Packet2d p)
    {
        return Mask2d(_mm_cmpneq_pd(p.reg, _mm_setzero_pd()));
    }

    inline Mask4d simdIsNonzero(Packet4d p)
    {
        return Mask4d(_mm256_cmp_pd(p.reg, _mm256_setzero_pd(), _CMP_NEQ_OQ));
    }

    inline Mask4i simdIsNonzero(Packet4i p)
    {
        return Mask4i(_mm_xor_si128(_mm_cmpeq_epi32(p.reg, _mm_setzero_si128()), _mm_set1_epi32(-1)));
    }

    inline Mask8i simdIsNonzero(Packet8i p)
    {
        return Mask8i(_mm256_xor_si256(_mm256_cmpeq_epi32(p.reg, _mm256_setzero_si256()), _mm256_set1_epi32(-1)));
    }

    inline bool simdAllFull(Mask4f m)
    {
        return _mm_movemask_ps(m.reg) == 0xF;
    }
    inline bool simdAnyFull(Mask4f m)
    {
        return _mm_movemask_ps(m.reg) != 0;
    }

    inline bool simdAllFull(Mask8f m)
    {
        return _mm256_movemask_ps(m.reg) == 0xFF;
    }
    inline bool simdAnyFull(Mask8f m)
    {
        return _mm256_movemask_ps(m.reg) != 0;
    }

    inline bool simdAllFull(Mask2d m)
    {
        return _mm_movemask_pd(m.reg) == 0x3;
    }
    inline bool simdAnyFull(Mask2d m)
    {
        return _mm_movemask_pd(m.reg) != 0;
    }

    inline bool simdAllFull(Mask4d m)
    {
        return _mm256_movemask_pd(m.reg) == 0xF;
    }
    inline bool simdAnyFull(Mask4d m)
    {
        return _mm256_movemask_pd(m.reg) != 0;
    }

    inline bool simdAllFull(Mask4i m)
    {
        return _mm_movemask_epi8(m.reg) == 0xFFFF;
    }
    inline bool simdAnyFull(Mask4i m)
    {
        return _mm_movemask_epi8(m.reg) != 0;
    }

    inline bool simdAllFull(Mask8i m)
    {
        return _mm256_movemask_epi8(m.reg) == 0xFFFFFFFF;
    }
    inline bool simdAnyFull(Mask8i m)
    {
        return _mm256_movemask_epi8(m.reg) != 0;
    }

}