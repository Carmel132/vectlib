#include <gtest/gtest.h>
#include <vect/vector.hpp>

#include <algorithm>
#include <cmath>
#include <numeric>
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
    EXPECT_TRUE(all(smul == smul2));

    auto sdiv = b / 2.0f;
    EXPECT_FLOAT_EQ(sdiv[0], 2.0f);
    EXPECT_FLOAT_EQ(sdiv[1], 1.5f);
    EXPECT_FLOAT_EQ(sdiv[2], 1.0f);
    EXPECT_FLOAT_EQ(sdiv[3], 0.5f);

    EXPECT_TRUE(all(a != b));
    EXPECT_FALSE(all(a == b));

    auto a_copy = a;
    EXPECT_TRUE(all(a_copy == a));
    EXPECT_FALSE(all(a_copy != a));

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
    EXPECT_TRUE(all(lerp0 == x));
    EXPECT_TRUE(all(lerp1 == y));


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

TEST(VectorMethodsTest, Map) {
    float4 v{1.0f, 0.0f, -3.0f, 2.0f};


    auto squared = map(v, [](float x) { return x * x; });
    EXPECT_FLOAT_EQ(squared[0], 1.0f);
    EXPECT_FLOAT_EQ(squared[2], 9.0f);
}

TEST(VectorMethodsTest, WhereOp) {
    float4 mask{1.0f, 0.0f, 1.0f, 0.0f};
    float4 true_vals{10.0f, 20.0f, 30.0f, 40.0f};
    float4 false_vals{100.0f, 200.0f, 300.0f, 400.0f};
    
    auto result = where(mask, true_vals, false_vals);
    EXPECT_FLOAT_EQ(result[0], 10.0f);  
    EXPECT_FLOAT_EQ(result[1], 200.0f); 
    EXPECT_FLOAT_EQ(result[2], 30.0f);  
    EXPECT_FLOAT_EQ(result[3], 400.0f); 
    
    
    float4 a{1.0f, 5.0f, 3.0f, 7.0f};
    float4 b{2.0f, 4.0f, 3.0f, 8.0f};
    auto where_result = where(a > b, a, b);
    EXPECT_FLOAT_EQ(where_result[0], 2.0f); 
    EXPECT_FLOAT_EQ(where_result[1], 5.0f); 
    EXPECT_FLOAT_EQ(where_result[2], 3.0f); 
    EXPECT_FLOAT_EQ(where_result[3], 8.0f); 
}

TEST(VectorMethodsTest, ReductionAllAny) {
    
    float4 all_true{1.0f, 2.0f, 3.0f, 4.0f};
    float4 has_false{1.0f, 0.0f, 3.0f, 4.0f};
    float4 all_false{0.0f, 0.0f, 0.0f, 0.0f};
    
    EXPECT_TRUE(all(all_true));
    EXPECT_FALSE(all(has_false));
    EXPECT_FALSE(all(all_false));
    
    
    EXPECT_TRUE(any(all_true));
    EXPECT_TRUE(any(has_false));
    EXPECT_FALSE(any(all_false));
    
    
    float4 a{1.0f, 2.0f, 3.0f, 4.0f};
    float4 b{0.5f, 2.5f, 3.0f, 5.0f};
    
    auto gt_mask = a > b; 
    EXPECT_FALSE(all(gt_mask));
    EXPECT_TRUE(any(gt_mask));
    
    auto eq_mask = a == a; 
    EXPECT_TRUE(all(eq_mask));
    EXPECT_TRUE(any(eq_mask));
    
    auto ne_mask = a != a; 
    EXPECT_FALSE(all(ne_mask));
    EXPECT_FALSE(any(ne_mask));
    
    
    int4 int_vec{1, 0, 1, 0};
    EXPECT_FALSE(all(int_vec));
    EXPECT_TRUE(any(int_vec));
    
    double2 double_vec{1.0, 0.0};
    EXPECT_FALSE(all(double_vec));
    EXPECT_TRUE(any(double_vec));
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

TEST(VectorSwizzleTest, BasicSwizzle) {

    float4 v{1.0f, 2.0f, 3.0f, 4.0f};


    auto sw_identity = v.swizzle<0,1,2,3>();
    EXPECT_FLOAT_EQ(sw_identity[0], 1.0f);
    EXPECT_FLOAT_EQ(sw_identity[1], 2.0f);
    EXPECT_FLOAT_EQ(sw_identity[2], 3.0f);
    EXPECT_FLOAT_EQ(sw_identity[3], 4.0f);


    auto sw_reorder = v.swizzle<1,0,3,2>();
    EXPECT_FLOAT_EQ(sw_reorder[0], 2.0f);
    EXPECT_FLOAT_EQ(sw_reorder[1], 1.0f);
    EXPECT_FLOAT_EQ(sw_reorder[2], 4.0f);
    EXPECT_FLOAT_EQ(sw_reorder[3], 3.0f);


    auto sw_dup = v.swizzle<0,0,1,1>();
    EXPECT_FLOAT_EQ(sw_dup[0], 1.0f);
    EXPECT_FLOAT_EQ(sw_dup[1], 1.0f);
    EXPECT_FLOAT_EQ(sw_dup[2], 2.0f);
    EXPECT_FLOAT_EQ(sw_dup[3], 2.0f);


    float2 v2{5.0f, 6.0f};
    auto sw2 = v2.swizzle<1,0>();
    EXPECT_FLOAT_EQ(sw2[0], 6.0f);
    EXPECT_FLOAT_EQ(sw2[1], 5.0f);


    auto sw_op = (v * 2.0f).swizzle<2,3,0,1>();
    EXPECT_FLOAT_EQ(sw_op[0], 6.0f);
    EXPECT_FLOAT_EQ(sw_op[1], 8.0f);
    EXPECT_FLOAT_EQ(sw_op[2], 2.0f);
    EXPECT_FLOAT_EQ(sw_op[3], 4.0f);


    int4 vi{1, 2, 3, 4};
    auto swi = vi.swizzle<3,2,1,0>();
    EXPECT_EQ(swi[0], 4);
    EXPECT_EQ(swi[1], 3);
    EXPECT_EQ(swi[2], 2);
    EXPECT_EQ(swi[3], 1);
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

TEST(UnalignedSimdTest, UnalignedLoadPacket) {

    float4 v{1.0f, 2.0f, 3.0f, 4.0f};
    

    auto aligned_packet = v.loadPacket(0);
    EXPECT_FLOAT_EQ(aligned_packet.vals[0], 1.0f);
    EXPECT_FLOAT_EQ(aligned_packet.vals[1], 2.0f);
    EXPECT_FLOAT_EQ(aligned_packet.vals[2], 3.0f);
    EXPECT_FLOAT_EQ(aligned_packet.vals[3], 4.0f);    auto unaligned_packet = v.loadPacketUnaligned(0);
    EXPECT_FLOAT_EQ(unaligned_packet.vals[0], 1.0f);
    EXPECT_FLOAT_EQ(unaligned_packet.vals[1], 2.0f);
    EXPECT_FLOAT_EQ(unaligned_packet.vals[2], 3.0f);
    EXPECT_FLOAT_EQ(unaligned_packet.vals[3], 4.0f);
}

TEST(UnalignedSimdTest, UnalignedStorePacket) {    float data[8] = {0};
    float4 v{5.0f, 6.0f, 7.0f, 8.0f};    auto packet = v.loadPacket(0);
    packet.store(&data[0]);
    EXPECT_FLOAT_EQ(data[0], 5.0f);
    EXPECT_FLOAT_EQ(data[1], 6.0f);
    EXPECT_FLOAT_EQ(data[2], 7.0f);
    EXPECT_FLOAT_EQ(data[3], 8.0f);    for (int i = 0; i < 8; ++i) data[i] = 0;
    packet.storeUnaligned(&data[0]);
    EXPECT_FLOAT_EQ(data[0], 5.0f);
    EXPECT_FLOAT_EQ(data[1], 6.0f);
    EXPECT_FLOAT_EQ(data[2], 7.0f);
    EXPECT_FLOAT_EQ(data[3], 8.0f);
}

TEST(UnalignedSimdTest, UnalignedArithmetic) {
    float4 a{1.0f, 2.0f, 3.0f, 4.0f};
    float4 b{5.0f, 6.0f, 7.0f, 8.0f};

    auto result = a + b;
    EXPECT_FLOAT_EQ(result[0], 6.0f);
    EXPECT_FLOAT_EQ(result[1], 8.0f);
    EXPECT_FLOAT_EQ(result[2], 10.0f);
    EXPECT_FLOAT_EQ(result[3], 12.0f);

    float4 c = a + b;  
    EXPECT_FLOAT_EQ(c[0], 6.0f);
    EXPECT_FLOAT_EQ(c[1], 8.0f);
    EXPECT_FLOAT_EQ(c[2], 10.0f);
    EXPECT_FLOAT_EQ(c[3], 12.0f);
}

TEST(UnalignedSimdTest, UnalignedSwizzleOperations) {
    float4 v{1.0f, 2.0f, 3.0f, 4.0f};

    auto swizzled = v.swizzle<3,2,1,0>();
    float4 result = swizzled;
    EXPECT_FLOAT_EQ(result[0], 4.0f);
    EXPECT_FLOAT_EQ(result[1], 3.0f);
    EXPECT_FLOAT_EQ(result[2], 2.0f);
    EXPECT_FLOAT_EQ(result[3], 1.0f);

    auto expr_swizzle = (v + v).swizzle<2,0,3,1>();
    float4 result2 = expr_swizzle;
    EXPECT_FLOAT_EQ(result2[0], 6.0f);
    EXPECT_FLOAT_EQ(result2[1], 2.0f);
    EXPECT_FLOAT_EQ(result2[2], 8.0f);
    EXPECT_FLOAT_EQ(result2[3], 4.0f);
}

TEST(UnalignedSimdTest, UnalignedDotProduct) {
    float4 x{1.0f, 0.0f, 0.0f, 0.0f};
    float4 y{0.0f, 1.0f, 0.0f, 0.0f};
    float4 z{2.0f, 3.0f, 4.0f, 5.0f};

    EXPECT_FLOAT_EQ(dot(x, y), 0.0f);
    EXPECT_FLOAT_EQ(dot(x, z), 2.0f);
    EXPECT_FLOAT_EQ(dot(y, z), 3.0f);
}

TEST(UnalignedSimdTest, UnalignedElementwiseOps) {
    float4 v{1.0f, 4.0f, 9.0f, 16.0f};

    auto sq_root = sqrt(v);
    EXPECT_NEAR(sq_root[0], 1.0f, kEps);
    EXPECT_NEAR(sq_root[1], 2.0f, kEps);
    EXPECT_NEAR(sq_root[2], 3.0f, kEps);
    EXPECT_NEAR(sq_root[3], 4.0f, kEps);

    auto abs_v = abs(v);
    EXPECT_FLOAT_EQ(abs_v[0], 1.0f);
    EXPECT_FLOAT_EQ(abs_v[1], 4.0f);
    EXPECT_FLOAT_EQ(abs_v[2], 9.0f);
    EXPECT_FLOAT_EQ(abs_v[3], 16.0f);
}

TEST(UnalignedSimdTest, UnalignedIntPacket) {
    int4 a{1, 2, 3, 4};
    int4 b{5, 6, 7, 8};

    auto packet_a = a.loadPacketUnaligned(0);
    int data[4] = {0};
    packet_a.storeUnaligned(data);
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(data[2], 3);
    EXPECT_EQ(data[3], 4);

    auto sum = a + b;
    EXPECT_EQ(sum[0], 6);
    EXPECT_EQ(sum[1], 8);
    EXPECT_EQ(sum[2], 10);
    EXPECT_EQ(sum[3], 12);
}

TEST(UnalignedSimdTest, UnalignedDoublePacket) {
    double2 a{1.5, 2.5};
    double2 b{3.5, 4.5};

    auto sum = a + b;
    EXPECT_DOUBLE_EQ(sum[0], 5.0);
    EXPECT_DOUBLE_EQ(sum[1], 7.0);

    auto packet = a.loadPacketUnaligned(0);
    double data[2] = {0};
    packet.storeUnaligned(data);
    EXPECT_DOUBLE_EQ(data[0], 1.5);
    EXPECT_DOUBLE_EQ(data[1], 2.5);
}

TEST(VectorSTLCompatibilityTest, RangeForAndSTDAlgorithms) {
    float4 a{1.0f, 5.0f, 3.0f, 7.0f};
    float4 b{2.0f, 4.0f, 3.0f, 8.0f};

    
    auto sum_expr = a + b; 
    float sum = 0.0f;
    for (auto v : sum_expr) {
        sum += v;
    }
    EXPECT_FLOAT_EQ(sum, 33.0f);

    
    auto acc = std::accumulate(sum_expr.begin(), sum_expr.end(), 0.0f);
    EXPECT_FLOAT_EQ(acc, 33.0f);

    
    std::vector<float> copied;
    copied.reserve(sum_expr.size());
    std::copy(sum_expr.begin(), sum_expr.end(), std::back_inserter(copied));
    ASSERT_EQ(copied.size(), 4);
    EXPECT_FLOAT_EQ(copied[0], 3.0f);
    EXPECT_FLOAT_EQ(copied[3], 15.0f);

    
    EXPECT_TRUE(std::all_of(sum_expr.begin(), sum_expr.end(), [](float v) { return v > 0.0f; }));
    EXPECT_TRUE(std::any_of(sum_expr.begin(), sum_expr.end(), [](float v) { return v == 15.0f; }));

    
    float4 mutable_v{2.0f, 0.5f, 4.0f, 1.5f};
    auto mutable_span = static_cast<std::span<float, 4>>(mutable_v);
    std::sort(mutable_span.begin(), mutable_span.end());
    EXPECT_EQ(mutable_v[0], 0.5f);
    EXPECT_EQ(mutable_v[3], 4.0f);

    
    std::vector<float> result;
    result.reserve(sum_expr.size());
    std::copy(sum_expr.begin(), sum_expr.end(), std::back_inserter(result));
    EXPECT_EQ(result.size(), sum_expr.size());
    EXPECT_FLOAT_EQ(result[0], 3.0f);
}

TEST(VectorComparisonLogicalTest, ComparisonWithLogicalOperators) {
    float4 a{1.0f, 5.0f, 3.0f, 7.0f};
    float4 b{2.0f, 4.0f, 3.0f, 8.0f};
    float4 c{0.0f, 6.0f, 2.0f, 9.0f};
    float4 d{3.0f, 3.0f, 4.0f, 6.0f};

    
    auto and_result = (a > b) && (c < d);
    
    
    
    EXPECT_FALSE(all(and_result));
    EXPECT_FALSE(any(and_result));

    
    auto or_result = (a > b) || (c < d);
    
    
    
    EXPECT_FALSE(all(or_result));
    EXPECT_TRUE(any(or_result));

    
    auto complex_result = (a >= b) && (c <= d);
    
    
    
    EXPECT_FALSE(all(complex_result));
    EXPECT_TRUE(any(complex_result));

    
    auto eq_and = (a == b) && (c == d);
    EXPECT_FALSE(all(eq_and));
    EXPECT_FALSE(any(eq_and));

    auto eq_or = (a == b) || (c == d);
    EXPECT_FALSE(all(eq_or));
    EXPECT_TRUE(any(eq_or));

    
    float4 true_vals{10.0f, 20.0f, 30.0f, 40.0f};
    float4 false_vals{100.0f, 200.0f, 300.0f, 400.0f};
    auto where_result = where((a > b) && (c < d), true_vals, false_vals);
    
    EXPECT_FLOAT_EQ(where_result[0], 100.0f);
    EXPECT_FLOAT_EQ(where_result[1], 200.0f);
    EXPECT_FLOAT_EQ(where_result[2], 300.0f);
    EXPECT_FLOAT_EQ(where_result[3], 400.0f);

    
    auto scalar_comp = (a > 2.0f) && (b < 6.0f);
    
    
    
    EXPECT_FALSE(all(scalar_comp));
    EXPECT_TRUE(any(scalar_comp));

    
    auto not_result = !(a > b);
    
    
    EXPECT_FALSE(all(not_result));
    EXPECT_TRUE(any(not_result));

    
    float4 bool_vec{0.0f, 1.0f, 0.0f, 1.0f}; 
    auto mask_from_vec = bool_vec != float4{0.0f, 0.0f, 0.0f, 0.0f}; 
    auto implicit_and = (a > b) && mask_from_vec;
    
    
    
    EXPECT_FALSE(all(implicit_and));
    EXPECT_TRUE(any(implicit_and));

    
    auto chained = (a > b) || ((c < d) && (a <= c));
    
    
    
    
    
    EXPECT_FALSE(all(chained));
    EXPECT_TRUE(any(chained));
}

TEST(VectorStructuredBindingTest, StructuredBinding) {
    float4 a{1.0f, 5.0f, 3.0f, 7.0f};
    float4 b{2.0f, 4.0f, 3.0f, 8.0f};
    float4 c{0.0f, 6.0f, 2.0f, 9.0f};
    const float4 d{3.0f, 3.0f, 4.0f, 6.0f};

    auto [x, y, z, w] = a;
    EXPECT_TRUE(all(a == float4{x, y, z, w}));

    auto [x2, y2, z2, w2] = a + b;
    EXPECT_TRUE(all((a + b) == float4{x2, y2, z2, w2}));

    auto [x3, y3, z3, w3] = c.swizzle<2, 0, 3, 1>();
    EXPECT_TRUE(all(c.swizzle<2, 0, 3, 1>() == float4{x3, y3, z3, w3}));

    auto [x4, y4, z4, w4] = d;
    EXPECT_TRUE(all(d == float4{x4, y4, z4, w4}));
    
    auto& [cx, cy, cz, cw] = d;
    static_assert(std::is_const_v<std::remove_reference_t<decltype(cx)>>, "Binding should be const");

    auto& [rx, ry, rz, rw] = a;
    rx = 10.0f;
    EXPECT_FLOAT_EQ(a[0], 10.0f);
}