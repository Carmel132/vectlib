#pragma once
#include <cmath>
#include <immintrin.h>
namespace vect::detail
{
    struct Packet8f
    {
        union
        {
            __m256 reg;
            float vals[8];
        };

        explicit Packet8f(__m256 r) : reg(r) {}
        Packet8f() : reg(_mm256_setzero_ps()) {}

        void store(float *dest) const
        {
            _mm256_store_ps(dest, reg);
        }
        void storeUnaligned(float *dest) const
        {
            _mm256_storeu_ps(dest, reg);
        }

        static auto load(const float *src) -> Packet8f
        {
            return Packet8f{_mm256_load_ps(src)};
        }
        static auto loadUnaligned(const float *src) -> Packet8f
        {
            return Packet8f{_mm256_loadu_ps(src)};
        }

        static auto broadcast(float val) -> Packet8f
        {
            return Packet8f{_mm256_set1_ps(val)};
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

    inline auto dot(Packet8f a, Packet8f b) -> float
    {
        __m256 prod = _mm256_mul_ps(a.reg, b.reg);
        __m256 sum1 = _mm256_hadd_ps(prod, prod);
        __m256 sum2 = _mm256_hadd_ps(sum1, sum1);
        __m128 sum3 = _mm256_extractf128_ps(sum2, 1);
        __m128 sum4 = _mm_add_ps(_mm256_castps256_ps128(sum2), sum3);
        return _mm_cvtss_f32(sum4);
    }

    inline auto max(Packet8f a, Packet8f b) -> Packet8f
    {
        return Packet8f{_mm256_max_ps(a.reg, b.reg)};
    }

    inline auto min(Packet8f a, Packet8f b) -> Packet8f
    {
        return Packet8f{_mm256_min_ps(a.reg, b.reg)};
    }

    inline auto sum(Packet8f a) -> float
    {
        __m256 sum1 = _mm256_hadd_ps(a.reg, a.reg);
        __m256 sum2 = _mm256_hadd_ps(sum1, sum1);
        __m128 sum3 = _mm256_extractf128_ps(sum2, 1);
        __m128 sum4 = _mm_add_ps(_mm256_castps256_ps128(sum2), sum3);
        return _mm_cvtss_f32(sum4);
    }

    inline auto sqrt(Packet8f a) -> Packet8f
    {
        return Packet8f{_mm256_sqrt_ps(a.reg)};
    }

    inline auto norm(Packet8f a) -> float
    {
        return std::sqrt(dot(a, a));
    }

    inline auto min_element(Packet8f a) -> float
    {
        __m256 min1 = _mm256_min_ps(a.reg, _mm256_shuffle_ps(a.reg, a.reg, _MM_SHUFFLE(1, 0, 3, 2)));
        __m256 min2 = _mm256_min_ps(min1, _mm256_shuffle_ps(min1, min1, _MM_SHUFFLE(2, 3, 0, 1)));
        __m128 min3 = _mm256_extractf128_ps(min2, 1);
        __m128 min4 = _mm_min_ps(_mm256_castps256_ps128(min2), min3);
        return _mm_cvtss_f32(min4);
    }

    inline auto max_element(Packet8f a) -> float
    {
        __m256 max1 = _mm256_max_ps(a.reg, _mm256_shuffle_ps(a.reg, a.reg, _MM_SHUFFLE(1, 0, 3, 2)));
        __m256 max2 = _mm256_max_ps(max1, _mm256_shuffle_ps(max1, max1, _MM_SHUFFLE(2, 3, 0, 1)));
        __m128 max3 = _mm256_extractf128_ps(max2, 1);
        __m128 max4 = _mm_max_ps(_mm256_castps256_ps128(max2), max3);
        return _mm_cvtss_f32(max4);
    }

    // Cross not implemented for 8f

    inline auto normalize(Packet8f a) -> Packet8f
    {
        float n = norm(a);
        if (n == 0.0f)
            return a;
        __m256 inv_n = _mm256_set1_ps(1.0f / n);
        return Packet8f{_mm256_mul_ps(a.reg, inv_n)};
    }

    inline auto all(Packet8f a) -> bool
    {
        __m256 zero = _mm256_setzero_ps();
        __m256 cmp = _mm256_cmp_ps(a.reg, zero, _CMP_NEQ_OQ);
        int mask = _mm256_movemask_ps(cmp);
        return mask == 255; // all 8 bits set
    }

    inline auto any(Packet8f a) -> bool
    {
        __m256 zero = _mm256_setzero_ps();
        __m256 cmp = _mm256_cmp_ps(a.reg, zero, _CMP_NEQ_OQ);
        int mask = _mm256_movemask_ps(cmp);
        return mask != 0;
    }
}