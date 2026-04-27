#pragma once
#include "vect/core/mat_expr.hpp"
#include "vect/core/vector.hpp"
#include "vect/detail/defer_eval.hpp"
#include "vect/detail/simd_traits.hpp"
#include "vect/expr/mat_column_view.hpp"
#include "vect/core/vec_expr.hpp"

namespace vect::core {

template <typename T, size_t R, size_t C>
class Matrix : public MatExpr<Matrix<T, R, C>, T, R, C> {
  std::array<Vector<T, detail::matrix_stride_v<T, C>>, R> rows_{};

  template <typename M>
  class MatRowView;
  friend MatRowView<Matrix<T, R, C>>;

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

  auto &getRow(size_t r) {
    return rows_[r];
  }
  auto getRow(size_t r) const {
    return MatRowView<Matrix<T, R, C>>(*this, r);
  }

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
        rows_[r] = expr.getRow(r);
      }
    }

    return *this;
  }

  
};

template <typename T, size_t R, size_t C>
template <typename M>
class Matrix<T, R, C>::MatRowView : public VecExpr<MatRowView<M>> {
    core::capture_t<M> m_;
        size_t rowIdx_;
    public:
        using valueType = typename M::valueType;
        static constexpr size_t dim = M::columns;

        MatRowView(const M& m, size_t r) : m_(m), rowIdx_(r) {}

        [[nodiscard]] auto operator[](size_t idx) const {
            return m_.at(rowIdx_, idx);
        }

        auto loadPacket(size_t idx) const {
            return m_.rows_[rowIdx_].loadPacket(idx);
        }

        [[nodiscard]] constexpr size_t size() const { return dim; }
    };
} // namespace vect::core
