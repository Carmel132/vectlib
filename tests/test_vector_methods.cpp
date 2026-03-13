#include <gtest/gtest.h>
#include <vect/vector.hpp>

#include <cmath>
#include <sstream>
#include <type_traits>

using namespace vect;
using namespace vect::expr;

static constexpr float kEps = 1e-6f;

TEST(VectorOperatorsTest, ArithmeticAndComparison) {
    float4 a{1.0f, -2.0f, 3.0f, -4.0f};
    float4 b{4.0f, 3.0f, 2.0f, 1.0f};


    auto add = a + b;
    EXPECT_FLOAT_EQ(add[0], 5.0f);
    EXPECT_FLOAT_EQ(add[1], 1.0f);
    EXPECT_FLOAT_EQ(add[2], 5.0f);
    EXPECT_FLOAT_EQ(add[3], -3.0f);

    auto sub = b - a;
    EXPECT_FLOAT_EQ(sub[0], 3.0f);
    EXPECT_FLOAT_EQ(sub[1], 5.0f);
    EXPECT_FLOAT_EQ(sub[2], -1.0f);
    EXPECT_FLOAT_EQ(sub[3], 5.0f);


    auto neg = -a;
    EXPECT_FLOAT_EQ(neg[0], -1.0f);
    EXPECT_FLOAT_EQ(neg[1], 2.0f);
    EXPECT_FLOAT_EQ(neg[2], -3.0f);
    EXPECT_FLOAT_EQ(neg[3], 4.0f);


    auto cmul = a * b;
    EXPECT_FLOAT_EQ(cmul[0], 4.0f);
    EXPECT_FLOAT_EQ(cmul[1], -6.0f);
    EXPECT_FLOAT_EQ(cmul[2], 6.0f);
    EXPECT_FLOAT_EQ(cmul[3], -4.0f);

    auto smul = a * 2.0f;
    EXPECT_FLOAT_EQ(smul[0], 2.0f);
    EXPECT_FLOAT_EQ(smul[1], -4.0f);
    EXPECT_FLOAT_EQ(smul[2], 6.0f);
    EXPECT_FLOAT_EQ(smul[3], -8.0f);

    auto smul2 = 2.0f * a;
    EXPECT_EQ(smul, smul2);

    auto sdiv = b / 2.0f;
    EXPECT_FLOAT_EQ(sdiv[0], 2.0f);
    EXPECT_FLOAT_EQ(sdiv[1], 1.5f);
    EXPECT_FLOAT_EQ(sdiv[2], 1.0f);
    EXPECT_FLOAT_EQ(sdiv[3], 0.5f);

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);

    auto a_copy = a;
    EXPECT_TRUE(a_copy == a);
    EXPECT_FALSE(a_copy != a);

    std::ostringstream oss;
    oss << a;
    EXPECT_EQ(oss.str(), "[1  -2  3  -4]");
}

TEST(VectorMethodsTest, DotCrossAndNorm) {
    float4 x{1.0f, 0.0f, 0.0f, 0.0f};
    float4 y{0.0f, 1.0f, 0.0f, 0.0f};
    float4 z{0.0f, 0.0f, 1.0f, 0.0f};

    EXPECT_FLOAT_EQ(dot(x, x), 1.0f);
    EXPECT_FLOAT_EQ(dot(x, y), 0.0f);
    EXPECT_FLOAT_EQ(dot(y, z), 0.0f);


    float3 x3{1.0f, 0.0f, 0.0f};
    float3 y3{0.0f, 1.0f, 0.0f};
    auto cx = cross(x3, y3);
    EXPECT_FLOAT_EQ(cx[0], 0.0f);
    EXPECT_FLOAT_EQ(cx[1], 0.0f);
    EXPECT_FLOAT_EQ(cx[2], 1.0f);

    float4 v{1.0f, 2.0f, 3.0f, 4.0f};
    EXPECT_FLOAT_EQ(sum(v), 10.0f);
    EXPECT_FLOAT_EQ(normSquared(v), dot(v, v));
    EXPECT_NEAR(norm(v), std::sqrt(dot(v, v)), kEps);

    auto n = normalize(v);
    EXPECT_NEAR(norm(n), 1.0f, kEps);


    auto lerp0 = lerp(x, y, 0.0f);
    auto lerp1 = lerp(x, y, 1.0f);
    EXPECT_TRUE(lerp0 == x);
    EXPECT_TRUE(lerp1 == y);


    auto proj = project(v, x);
    EXPECT_NEAR(proj[0], v[0], kEps);
    EXPECT_NEAR(proj[1], 0.0f, kEps);

    auto rej = reject(v, x);
    EXPECT_NEAR(rej[0], 0.0f, kEps);
    EXPECT_NEAR(rej[1], v[1], kEps);

    auto refl = reflect(x, y);

    EXPECT_NEAR(refl[0], x[0], kEps);
    EXPECT_NEAR(refl[1], x[1], kEps);
}

TEST(VectorMethodsTest, ElementwiseMath) {

    float2 v{1.25f, -2.5f};

    auto abs_v = abs(v);
    EXPECT_FLOAT_EQ(abs_v[0], 1.25f);
    EXPECT_FLOAT_EQ(abs_v[1], 2.5f);

    auto sqrt_v = sqrt(abs_v);
    EXPECT_NEAR(sqrt_v[0], std::sqrt(1.25f), kEps);

    auto exp_v = exp(v);
    EXPECT_NEAR(exp_v[0], std::exp(1.25f), kEps);

    auto log_v = log(abs_v);
    EXPECT_NEAR(log_v[0], std::log(1.25f), kEps);

    auto neg_v = negate(v);
    EXPECT_FLOAT_EQ(neg_v[0], -1.25f);
    EXPECT_FLOAT_EQ(neg_v[1], 2.5f);


    auto sin_v = sin(v);
    auto cos_v = cos(v);
    auto tan_v = tan(v);
    EXPECT_NEAR(tan_v[0], std::tan(v[0]), kEps);


    float2 unit{-1.0f, 1.0f};
    auto asin_v = asin(unit);
    auto acos_v = acos(unit);
    auto atan_v = atan(unit);
    EXPECT_NEAR(asin_v[0], std::asin(-1.0f), kEps);
    EXPECT_NEAR(acos_v[1], std::acos(1.0f), kEps);
    EXPECT_NEAR(atan_v[0], std::atan(-1.0f), kEps);

    auto floor_v = floor(v);
    auto ceil_v = ceil(v);
    auto round_v = round(v);
    EXPECT_FLOAT_EQ(floor_v[0], 1.0f);
    EXPECT_FLOAT_EQ(ceil_v[0], 2.0f);
    EXPECT_FLOAT_EQ(round_v[0], 1.0f);

    auto sign_v = sign(v);
    EXPECT_EQ(sign_v[0], 1);
    EXPECT_EQ(sign_v[1], -1);

    auto fract_v = fract(v);
    EXPECT_NEAR(fract_v[0], 0.25f, kEps);

    auto clamp_v = clamp(v, -1.0f, 1.0f);
    EXPECT_FLOAT_EQ(clamp_v[0], 1.0f);
    EXPECT_FLOAT_EQ(clamp_v[1], -1.0f);
}

TEST(VectorMethodsTest, MapAllAny) {
    float4 v{1.0f, 0.0f, -3.0f, 2.0f};


    auto squared = map(v, [](float x) { return x * x; });
    EXPECT_FLOAT_EQ(squared[0], 1.0f);
    EXPECT_FLOAT_EQ(squared[2], 9.0f);


    float4 allFalse{0.0f, 0.0f, 0.0f, 0.0f};
    EXPECT_FALSE(any(allFalse));
    EXPECT_TRUE(any(v));

    EXPECT_FALSE(all(v));


    float2 v2{1.0f, 1.0f};
    EXPECT_TRUE(all(v2));
}

TEST(VectorMethodsTest, NormalizeZeroLength) {
    float4 zero{0.0f, 0.0f, 0.0f, 0.0f};
    auto n = normalize(zero);


    for (size_t i = 0; i < zero.size(); ++i) {
        EXPECT_TRUE(std::isnan(n[i]) || std::isinf(n[i]));
    }
}

TEST(VectorOperatorsTest, AliasingAssignment) {
    float4 v{1.0f, 2.0f, 3.0f, 4.0f};


    v = v + v;
    EXPECT_FLOAT_EQ(v[0], 2.0f);
    EXPECT_FLOAT_EQ(v[1], 4.0f);
    EXPECT_FLOAT_EQ(v[2], 6.0f);
    EXPECT_FLOAT_EQ(v[3], 8.0f);
}

TEST(SimdTraitsTest, VectorAlignment) {

    EXPECT_EQ(alignof(float4), (detail::SimdTraits<float, 4>::alignment));
    EXPECT_EQ(alignof(float2), (detail::SimdTraits<float, 2>::alignment));
}

template <typename T, size_t N>
void ValidateSimdTraits() {
    using Traits = detail::SimdTraits<T, N>;


    EXPECT_TRUE(Traits::width == 1 || Traits::width == 2 || Traits::width == 4 || Traits::width == 8);


    if constexpr (Traits::available) {
        EXPECT_GT(Traits::width, 1u);
    } else {
        EXPECT_EQ(Traits::width, 1u);
    }


    EXPECT_LE(Traits::alignment, Traits::simdAlign);
    EXPECT_LE(Traits::alignment, Traits::scalarAlign);
}

TEST(SimdTraitsTest, FloatVariants) {
    ValidateSimdTraits<float, 1>();
    ValidateSimdTraits<float, 2>();
    ValidateSimdTraits<float, 3>();
    ValidateSimdTraits<float, 4>();
    ValidateSimdTraits<float, 8>();
}

TEST(SimdTraitsTest, DoubleVariants) {
    ValidateSimdTraits<double, 1>();
    ValidateSimdTraits<double, 2>();
    ValidateSimdTraits<double, 3>();
    ValidateSimdTraits<double, 4>();
}

TEST(SimdTraitsTest, IntVariants) {
    ValidateSimdTraits<int, 1>();
    ValidateSimdTraits<int, 2>();
    ValidateSimdTraits<int, 3>();
    ValidateSimdTraits<int, 4>();
    ValidateSimdTraits<int, 8>();
}
