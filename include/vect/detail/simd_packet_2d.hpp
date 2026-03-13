#pragma once
#include <immintrin.h>
namespace vect::detail {
    struct Packet2d {
        __m128d reg;

        void store(double* dest) const 
        {
            _mm_store_pd(dest, reg);
        }

        friend auto operator+(Packet2d a, Packet2d b) -> Packet2d
        {
            return {_mm_add_pd(a.reg, b.reg)};
        }
        friend auto operator-(Packet2d a, Packet2d b) -> Packet2d
        {
            return {_mm_sub_pd(a.reg, b.reg)};
        }
        friend auto operator*(Packet2d a, Packet2d b) -> Packet2d
        {
            return {_mm_mul_pd(a.reg, b.reg)};
        }
        friend auto operator/(Packet2d a, Packet2d b) -> Packet2d
        {
            return {_mm_div_pd(a.reg, b.reg)};
        }
        friend auto operator-(Packet2d a) -> Packet2d
        {
            return {_mm_sub_pd(_mm_setzero_pd(), a.reg)};
        }
    };

}