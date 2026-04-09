#include <gtest/gtest.h>

#include "vect/core/matrix.hpp"
#include "vect/expr/mat_method.hpp"
#include "vect/expr/mat_operator.hpp"
#include "vect/vector.hpp"

using namespace vect;
using namespace vect::expr;
using namespace vect::core;

static constexpr float kEps = 1e-6f;

TEST(MatrixOperatorsTest, AddSubtractNegateScalar) {
  Matrix<float, 2, 3> a{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

  Matrix<float, 2, 3> b{6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};

  auto sum = a + b;
  EXPECT_FLOAT_EQ(sum.at(0, 0), 7.0f);
  EXPECT_FLOAT_EQ(sum.at(0, 2), 7.0f);
  EXPECT_FLOAT_EQ(sum.at(1, 0), 7.0f);
  EXPECT_FLOAT_EQ(sum.at(1, 2), 7.0f);

  auto diff = a - b;
  EXPECT_FLOAT_EQ(diff.at(0, 0), -5.0f);
  EXPECT_FLOAT_EQ(diff.at(0, 2), -1.0f);
  EXPECT_FLOAT_EQ(diff.at(1, 0), 1.0f);
  EXPECT_FLOAT_EQ(diff.at(1, 2), 5.0f);

  auto neg = -a;
  EXPECT_FLOAT_EQ(neg.at(0, 0), -1.0f);
  EXPECT_FLOAT_EQ(neg.at(1, 2), -6.0f);

  auto scaled = a * 2.0f;
  EXPECT_FLOAT_EQ(scaled.at(0, 0), 2.0f);
  EXPECT_FLOAT_EQ(scaled.at(1, 2), 12.0f);

  auto scaled2 = 2.0f * a;
  EXPECT_FLOAT_EQ(scaled2.at(0, 1), 4.0f);
  EXPECT_FLOAT_EQ(scaled2.at(1, 2), 12.0f);
}

TEST(MatrixMethodsTest, RowColumnTransposeAndFill) {
  Matrix<float, 2, 3> m{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

  EXPECT_FLOAT_EQ(m.at(1, 0), 4.0f);
  EXPECT_FLOAT_EQ(m(0, 2), 3.0f);

  auto row0 = m.getRow(0);
  EXPECT_FLOAT_EQ(row0[0], 1.0f);
  EXPECT_FLOAT_EQ(row0[2], 3.0f);

  auto col1 = m.getColumn(1);
  EXPECT_FLOAT_EQ(col1[0], 2.0f);
  EXPECT_FLOAT_EQ(col1[1], 5.0f);

  auto t = transpose(m);
  Matrix<float, 3, 2> transposed = t;
  EXPECT_FLOAT_EQ(transposed.at(0, 1), 4.0f);
  EXPECT_FLOAT_EQ(transposed.at(2, 1), 6.0f);

  Matrix<float, 2, 2> fill_mat;
  fill_mat.fill(7.0f);
  EXPECT_FLOAT_EQ(fill_mat.at(0, 0), 7.0f);
  EXPECT_FLOAT_EQ(fill_mat.at(1, 1), 7.0f);
}

TEST(MatrixMethodsTest, MatrixDiagonalTest) {
  Matrix<float, 3, 3> square{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto square_diag_test = vect::expr::diag(square);
  vect::Vector<float, 3> square_diag_true{1, 5, 9};
  EXPECT_TRUE(square_diag_test.size() == 3);
  EXPECT_TRUE(all(square_diag_test == square_diag_true));

  Matrix<float, 2, 3> horiz{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto horiz_diag_test = vect::expr::diag(horiz);
  vect::Vector<float, 2> horiz_diag_true{1, 5};
  EXPECT_TRUE(horiz_diag_test.size() == 2);
  EXPECT_TRUE(all(horiz_diag_test == horiz_diag_true));

  Matrix<float, 3, 2> vert{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto vert_diag_test = vect::expr::diag(vert);
  vect::Vector<float, 2> vert_diag_true{1, 4};
  EXPECT_TRUE(vert_diag_test.size() == 2);
  EXPECT_TRUE(all(vert_diag_test == vert_diag_true));
}

TEST(MatrixOperatorsTest, MatrixVectorAndVectorMatrixMultiplication) {
  Matrix<float, 2, 3> m{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
  float3 v{7.0f, 8.0f, 9.0f};

  auto m_times_v = m * v;
  EXPECT_FLOAT_EQ(m_times_v[0], 50.0f);
  EXPECT_FLOAT_EQ(m_times_v[1], 122.0f);

  float2 u{1.0f, 2.0f};
  auto u_times_m = u * m;
  EXPECT_FLOAT_EQ(u_times_m[0], 9.0f);
  EXPECT_FLOAT_EQ(u_times_m[1], 12.0f);
  EXPECT_FLOAT_EQ(u_times_m[2], 15.0f);
}

TEST(MatrixMethodsTest, ComponentWiseUnaryOperations) {
  Matrix<float, 2, 2> m{1.0f, 4.0f, 9.0f, 16.0f};

  auto m_sqrt = sqrt(m);
  EXPECT_NEAR(m_sqrt.at(0, 0), 1.0f, kEps);
  EXPECT_NEAR(m_sqrt.at(0, 1), 2.0f, kEps);
  EXPECT_NEAR(m_sqrt.at(1, 0), 3.0f, kEps);
  EXPECT_NEAR(m_sqrt.at(1, 1), 4.0f, kEps);

  Matrix<float, 2, 2> negative{-1.0f, 2.0f, -3.0f, 4.0f};
  auto m_abs = abs(negative);
  EXPECT_NEAR(m_abs.at(0, 0), 1.0f, kEps);
  EXPECT_NEAR(m_abs.at(1, 0), 3.0f, kEps);

  Matrix<float, 2, 2> exp_input{0.0f, 1.0f, 2.0f, 3.0f};
  auto m_exp = exp(exp_input);
  EXPECT_NEAR(m_exp.at(0, 0), std::exp(0.0f), kEps);
  EXPECT_NEAR(m_exp.at(1, 1), std::exp(3.0f), kEps);

  Matrix<float, 2, 2> log_input{1.0f, std::exp(1.0f), std::exp(2.0f),
                                std::exp(3.0f)};
  auto m_log = log(log_input);
  EXPECT_NEAR(m_log.at(0, 0), 0.0f, kEps);
  EXPECT_NEAR(m_log.at(1, 1), 3.0f, kEps);
}

TEST(MatrixOperatorsTest, MatrixMatrixMultiplicationAndAssignment) {
  Matrix<float, 2, 3> a{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
  Matrix<float, 3, 2> b{7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f};

  auto product = a * b;

  EXPECT_FLOAT_EQ(product.at(0, 0), 58.0f);
  EXPECT_FLOAT_EQ(product.at(0, 1), 64.0f);
  EXPECT_FLOAT_EQ(product.at(1, 0), 139.0f);
  EXPECT_FLOAT_EQ(product.at(1, 1), 154.0f);
}

TEST(MatrixOperatorsTest, AliasingAssignment) {
  Matrix<float, 2, 2> m{1.0f, 2.0f, 3.0f, 4.0f};

  m = m + m;
  EXPECT_FLOAT_EQ(m.at(0, 0), 2.0f);
  EXPECT_FLOAT_EQ(m.at(0, 1), 4.0f);
  EXPECT_FLOAT_EQ(m.at(1, 0), 6.0f);
  EXPECT_FLOAT_EQ(m.at(1, 1), 8.0f);

  m = m * 2.0f;
  EXPECT_FLOAT_EQ(m.at(0, 0), 4.0f);
  EXPECT_FLOAT_EQ(m.at(0, 1), 8.0f);
  EXPECT_FLOAT_EQ(m.at(1, 0), 12.0f);
  EXPECT_FLOAT_EQ(m.at(1, 1), 16.0f);
}

TEST(MatrixReductionTest, RowAndColumnReductions) {
  Matrix<float, 2, 3> m{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

  auto row_min = rowMin(m);
  EXPECT_FLOAT_EQ(row_min[0], 1.0f);
  EXPECT_FLOAT_EQ(row_min[1], 4.0f);

  auto row_max = rowMax(m);
  EXPECT_FLOAT_EQ(row_max[0], 3.0f);
  EXPECT_FLOAT_EQ(row_max[1], 6.0f);

  auto row_sum = rowSum(m);
  EXPECT_FLOAT_EQ(row_sum[0], 6.0f);
  EXPECT_FLOAT_EQ(row_sum[1], 15.0f);

  auto row_mean = rowMean(m);
  EXPECT_FLOAT_EQ(row_mean[0], 2.0f);
  EXPECT_FLOAT_EQ(row_mean[1], 5.0f);

  auto row_product = rowProduct(m);
  EXPECT_FLOAT_EQ(row_product[0], 6.0f);
  EXPECT_FLOAT_EQ(row_product[1], 120.0f);

  auto col_sum = columnSum(m);
  EXPECT_FLOAT_EQ(col_sum[0], 5.0f);
  EXPECT_FLOAT_EQ(col_sum[1], 7.0f);
  EXPECT_FLOAT_EQ(col_sum[2], 9.0f);

  auto col_min = columnMin(m);
  EXPECT_FLOAT_EQ(col_min[0], 1.0f);
  EXPECT_FLOAT_EQ(col_min[1], 2.0f);
  EXPECT_FLOAT_EQ(col_min[2], 3.0f);

  auto col_max = columnMax(m);
  EXPECT_FLOAT_EQ(col_max[0], 4.0f);
  EXPECT_FLOAT_EQ(col_max[1], 5.0f);
  EXPECT_FLOAT_EQ(col_max[2], 6.0f);

  auto col_mean = columnMean(m);
  EXPECT_FLOAT_EQ(col_mean[0], 2.5f);
  EXPECT_FLOAT_EQ(col_mean[1], 3.5f);
  EXPECT_FLOAT_EQ(col_mean[2], 4.5f);

  auto col_product = columnProduct(m);
  EXPECT_FLOAT_EQ(col_product[0], 4.0f);
  EXPECT_FLOAT_EQ(col_product[1], 10.0f);
  EXPECT_FLOAT_EQ(col_product[2], 18.0f);
}