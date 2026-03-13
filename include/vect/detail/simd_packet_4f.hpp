#pragma once
#include <immintrin.h>
#include <cmath>
namespace vect::detail
{
    struct Packet4f
    {
        union
        {
            __m128 reg;
            float vals[4];
        };

        void store(float *dest) const
        {
            _mm_store_ps(dest, reg);
        }

        friend auto operator+(Packet4f a, Packet4f b) -> Packet4f
        {
            return {_mm_add_ps(a.reg, b.reg)};
        }
        friend auto operator-(Packet4f a, Packet4f b) -> Packet4f
        {
            return {_mm_sub_ps(a.reg, b.reg)};
        }
        friend auto operator*(Packet4f a, Packet4f b) -> Packet4f
        {
            return {_mm_mul_ps(a.reg, b.reg)};
        }
        friend auto operator/(Packet4f a, Packet4f b) -> Packet4f
        {
            return {_mm_div_ps(a.reg, b.reg)};
        }
        friend auto operator-(Packet4f a) -> Packet4f
        {
            return {_mm_sub_ps(_mm_setzero_ps(), a.reg)};
        }
    };

    inline auto dot(Packet4f a, Packet4f b) -> float
    {
        __m128 prod = _mm_mul_ps(a.reg, b.reg);
        __m128 sum1 = _mm_hadd_ps(prod, prod);
        __m128 sum2 = _mm_hadd_ps(sum1, sum1);
        return _mm_cvtss_f32(sum2);
    }

    inline auto sum(Packet4f a) -> float
    {
        __m128 sum1 = _mm_hadd_ps(a.reg, a.reg);
        __m128 sum2 = _mm_hadd_ps(sum1, sum1);
        return _mm_cvtss_f32(sum2);
    }

    inline auto sqrt(Packet4f a) -> Packet4f
    {
        return {_mm_sqrt_ps(a.reg)};
    }

    inline auto norm(Packet4f a) -> float
    {
        return std::sqrt(dot(a, a));
    }

    inline auto min_element(Packet4f a) -> float
    {
        __m128 min1 = _mm_min_ps(a.reg, _mm_shuffle_ps(a.reg, a.reg, _MM_SHUFFLE(1, 0, 3, 2)));
        __m128 min2 = _mm_min_ps(min1, _mm_shuffle_ps(min1, min1, _MM_SHUFFLE(2, 3, 0, 1)));
        return _mm_cvtss_f32(min2);
    }

    inline auto max_element(Packet4f a) -> float
    {
        __m128 max1 = _mm_max_ps(a.reg, _mm_shuffle_ps(a.reg, a.reg, _MM_SHUFFLE(1, 0, 3, 2)));
        __m128 max2 = _mm_max_ps(max1, _mm_shuffle_ps(max1, max1, _MM_SHUFFLE(2, 3, 0, 1)));
        return _mm_cvtss_f32(max2);
    }

    inline auto cross(Packet4f a, Packet4f b) -> Packet4f
    {
        __m128 cross = _mm_sub_ps(
            _mm_mul_ps(_mm_shuffle_ps(a.reg, a.reg, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b.reg, b.reg, _MM_SHUFFLE(3, 1, 0, 2))),
            _mm_mul_ps(_mm_shuffle_ps(a.reg, a.reg, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b.reg, b.reg, _MM_SHUFFLE(3, 0, 2, 1))));
        return {cross};
    }

    inline auto normalize(Packet4f a) -> Packet4f
    {
        float n = norm(a);
        if (n == 0.0f)
            return a;
        __m128 inv_n = _mm_set1_ps(1.0f / n);
        return {_mm_mul_ps(a.reg, inv_n)};
    }

    inline bool all(Packet4f a) {
        __m128 zero = _mm_setzero_ps();
        __m128 cmp = _mm_cmpneq_ps(a.reg, zero);
        int mask = _mm_movemask_ps(cmp);
        return mask == 15; // all 4 bits set
    }

    inline bool any(Packet4f a) {
        __m128 zero = _mm_setzero_ps();
        __m128 cmp = _mm_cmpneq_ps(a.reg, zero);
        int mask = _mm_movemask_ps(cmp);
        return mask != 0;
    }
}