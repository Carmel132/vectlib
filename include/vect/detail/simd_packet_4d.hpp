#pragma once
#include <immintrin.h>
namespace vect::detail
{
    struct Packet4d {
        __m256d reg;
        void store(double* dest) const 
        {
            _mm256_store_pd(dest, reg);
        }
        friend auto operator+(Packet4d a, Packet4d b) -> Packet4d
        {
            return {_mm256_add_pd(a.reg, b.reg)};
        }
        friend auto operator-(Packet4d a, Packet4d b) -> Packet4d
        {
            return {_mm256_sub_pd(a.reg, b.reg)};
        }
        friend auto operator*(Packet4d a, Packet4d b) -> Packet4d
        {
            return {_mm256_mul_pd(a.reg, b.reg)};
        }
        friend auto operator/(Packet4d a, Packet4d b) -> Packet4d
        {
            return {_mm256_div_pd(a.reg, b.reg)};
        }
        friend auto operator-(Packet4d a) -> Packet4d
        {
            return {_mm256_sub_pd(_mm256_setzero_pd(), a.reg)};
        }
    };
}