#pragma once
#include <immintrin.h>
namespace vect::detail
{
    struct Packet8f
    {
        __m256 reg;

        explicit Packet8f(__m256 r) : reg(r) {}
        Packet8f() : reg(_mm256_setzero_ps()) {}

        void store(float *dest) const
        {
            _mm256_store_ps(dest, reg);
        }

        friend auto operator+(Packet8f a, Packet8f b) -> Packet8f
        {
            return Packet8f{_mm256_add_ps(a.reg, b.reg)};
        }
        friend auto operator-(Packet8f a, Packet8f b) -> Packet8f
        {
            return Packet8f{_mm256_sub_ps(a.reg, b.reg)};
        }
        friend auto operator*(Packet8f a, Packet8f b) -> Packet8f
        {
            return Packet8f{_mm256_mul_ps(a.reg, b.reg)};
        }
        friend auto operator/(Packet8f a, Packet8f b) -> Packet8f
        {
            return Packet8f{_mm256_div_ps(a.reg, b.reg)};
        }
        friend auto operator-(Packet8f a) -> Packet8f
        {
            return Packet8f{_mm256_sub_ps(_mm256_setzero_ps(), a.reg)};
        }
    };
}