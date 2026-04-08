#pragma once
#include "vect/core/mat_expr.hpp"
#include "vect/core/vector.hpp"
#include "vect/detail/defer_eval.hpp"
#include "vect/detail/simd_traits.hpp"
#include "vect/expr/mat_column_view.hpp"
namespace vect::core {

template <typename T, size_t R, size_t C>
class Matrix : public MatExpr<Matrix<T, R, C>, T, R, C> {
  std::array<Vector<T, detail::matrix_stride_v<T, C>>, R> rows_{};

public:
  Matrix() = default;

  Matrix(std::initializer_list<T> list) {
    auto it = list.begin();
    for (size_t r = 0; r < R; ++r) {
      for (size_t c = 0; c < C; ++c) {
        if (it != list.end()) {
          rows_[r][c] = *it++;
        }
      }
    }
  }

  template <typename Expr> Matrix(const MatExpr<Expr, T, R, C> &expr) {
    *this = expr;
  }

  const T &at(size_t r, size_t c) const { return rows_[r][c]; }
  T &at(size_t r, size_t c) { return rows_[r][c]; }

  auto &getRow(size_t r) { return rows_[r]; }
  const auto &getRow(size_t r) const { return rows_[r]; }

  [[nodiscard]] auto getColumn(size_t c) const {
    return vect::expr::MatColumnView<Matrix<T, R, C>>(*this, c);
  }

  const T &operator()(size_t r, size_t c) const { return rows_[r][c]; }
  T &operator()(size_t r, size_t c) { return rows_[r][c]; }

  void fill(T value) {
    for (size_t idx = 0; idx < R; ++idx) {
      rows_[idx].fill(value);
    }
  }

  template <typename E> Matrix &operator=(const MatExpr<E, T, R, C> &expr) {
    if constexpr (detail::DeferAssign<E>::value) {
      this->fill(0);
      expr.self().evaluateTo(*this);
    } else {
      for (size_t r = 0; r < R; ++r) {
        rows_[r] = expr.row(r);
      }
    }

    return *this;
  }
};
} // namespace vect::core