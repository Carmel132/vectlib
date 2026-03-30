#pragma once


#include "vect/detail/simd_mask.hpp"

namespace vect::detail {
    inline bool simdAllFull(Mask4f m) {
        return _mm_movemask_ps(m.reg) == 0xF;
    }
    inline bool simdAnyFull(Mask4f m) {
        return _mm_movemask_ps(m.reg) != 0;
    }

    inline bool simdAllFull(Mask8f m) {
        return _mm256_movemask_ps(m.reg) == 0xFF;
    }
    inline bool simdAnyFull(Mask8f m) {
        return _mm256_movemask_ps(m.reg) != 0;
    }

    inline bool simdAllFull(Mask2d m) {
        return _mm_movemask_pd(m.reg) == 0x3;
    }
    inline bool simdAnyFull(Mask2d m) {
        return _mm_movemask_pd(m.reg) != 0;
    }

    inline bool simdAllFull(Mask4d m) {
        return _mm256_movemask_pd(m.reg) == 0xF;
    }
    inline bool simdAnyFull(Mask4d m) {
        return _mm256_movemask_pd(m.reg) != 0;
    }

    inline bool simdAllFull(Mask4i m) {
        return _mm_movemask_epi8(m.reg) == 0xFFFF;
    }
    inline bool simdAnyFull(Mask4i m) {
        return _mm_movemask_epi8(m.reg) != 0;
    }

    inline bool simdAllFull(Mask8i m) {
        return _mm256_movemask_epi8(m.reg) == 0xFFFFFFFF;
    }
    inline bool simdAnyFull(Mask8i m) {
        return _mm256_movemask_epi8(m.reg) != 0;
    }

}