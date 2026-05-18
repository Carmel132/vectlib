#pragma once

#include "vect/core/mat_expr.hpp"
#include "vect/detail/simd_comparison.hpp"
#include "vect/expr/mat_binary_op.hpp"
#include "vect/expr/mat_method.hpp"
#include "vect/expr/mat_unary_op.hpp"
#include "vect/expr/vector_method.hpp"
#include <functional>

namespace vect::expr {
template <typename L, typename R>
class MatVecMultExpr : public core::VecExpr<MatVecMultExpr<L, R>> {
  core::capture_t<L> l_;
  core::capture_t<R> r_;

public:
  using valueType =
      std::common_type_t<typename L::valueType, typename R::valueType>;
  static constexpr size_t dim = L::rows;

  MatVecMultExpr(const L &l, const R &r) : l_{l}, r_{r} {
    static_assert(
        L::columns == R::dim,
        "Matrix columns must match vector dimension for multiplication");
  }

  [[nodiscard]] auto operator[](size_t idx) const -> valueType {
    valueType sum = valueType{};

    auto row = l_.getRow(idx);
    for (size_t c = 0; c < L::columns; ++c) {
      sum += row[c] * r_[c];
    }
    return sum;
  }

  [[nodiscard]] auto size() const -> size_t { return dim; }

  [[nodiscard]] auto loadPacket(size_t idx) const {
    return loadPacketUnaligned(idx);
  }

  [[nodiscard]] auto loadPacketUnaligned(size_t idx) const {
    using Traits = detail::SimdTraits<valueType, dim>;
    using Packet = typename Traits::packetType;

    alignas(Traits::alignment) valueType vals[Traits::width];

    for (size_t i = 0; i < Traits::width; ++i) {
      if (idx + +i < dim) {
        vals[i] = expr::dot(l_.getRow(idx + i), r_);
      } else {
        vals[i] = 0;
      }
    }

    return Packet::load(vals);
  }
};

template <typename L, typename R>
class VecMatMultExpr : public core::VecExpr<VecMatMultExpr<L, R>> {
  core::capture_t<L> l_;
  core::capture_t<R> r_;

public:
  static constexpr size_t dim = R::columns;
  using valueType = typename L::valueType;

  VecMatMultExpr(const L &v, const R &m) : l_(v), r_(m) {
    static_assert(L::dim == R::rows,
                  "Vector dimension must match matrix rows for multiplication");
  }

  void evaluateTo(auto &dest) const {
    dest.fill(0);
    for (size_t i = 0; i < L::dim; ++i) {
      dest.multiplyAccumulate(l_[i], r_.getRow(i));
    }
  }

  valueType operator[](size_t idx) const { return dot(l_, r_.getColumn(idx)); }
};

template <typename L, typename R>
class MatMatMultExpr
    : public core ::MatExpr<MatMatMultExpr<L, R>, typename L::valueType,
                            L::rows, R::columns> {
  core::capture_t<L> l_;
  core::capture_t<R> r_;

public:
  using T = typename L::valueType;

  MatMatMultExpr(const L &l, const R &r) : l_(l), r_(r) {}

  [[nodiscard]] constexpr T at(size_t r, size_t c) const {
    return dot(l_.getRow(r), r_.getColumn(c));
  }

  template <typename Dest> void evaluateTo(Dest &C) const {
    C.fill(0);
    for (size_t k = 0; k < L::columns; ++k) {
      auto colA = l_.getColumn(k);
      auto rowB = r_.getRow(k);

      for (size_t i = 0; i < L::rows; ++i) {
        C.getRow(i).multiplyAccumulate(colA[i], rowB);
      }
    }
  }
};
} // namespace vect::expr

namespace vect::core {
// Matrix addition
template <IsMatExpr L, IsMatExpr R> auto operator+(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, std::plus<>>(l, r);
}
template <IsMatExpr M, IsVecExpr V> auto operator+(const M &m, const V &v) {
  // Prioritize row-wise broadcasting
  if constexpr (M::rows == V::dim) {
    return m + expr::broadcastTo<M::columns, expr::BroadcastAxis::COLUMN>(v);
  } else if constexpr (M::columns == V::dim) {
    return m + expr::broadcastTo<M::rows, expr::BroadcastAxis::ROW>(v);
  }
}
template <IsMatExpr M, IsVecExpr V> auto operator+(const V &v, const M &m) {
  if constexpr (M::rows == V::dim) {
    return expr::broadcastTo<M::columns, expr::BroadcastAxis::COLUMN>(v) + m;
  } else if constexpr (M::columns == V::dim) {
    return expr::broadcastTo<M::rows, expr::BroadcastAxis::ROW>(v) + m;
  }
}

template <IsMatExpr L, IsMatExpr R> auto operator-(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, std::minus<>>(l, r);
}
template <IsMatExpr M, IsVecExpr V> auto operator-(const M &m, const V &v) {
  if constexpr (M::rows == V::dim) {
    return m - expr::broadcastTo<M::columns, expr::BroadcastAxis::COLUMN>(v);
  } else if constexpr (M::columns == V::dim) {
    return m - expr::broadcastTo<M::rows, expr::BroadcastAxis::ROW>(v);
  }
}
template <IsMatExpr M, IsVecExpr V> auto operator-(const V &v, const M &m) {
  if constexpr (M::rows == V::dim) {
    return expr::broadcastTo<M::columns, expr::BroadcastAxis::COLUMN>(v) - m;
  } else if constexpr (M::columns == V::dim) {
    return expr::broadcastTo<M::rows, expr::BroadcastAxis::ROW>(v) - m;
  }
}

template <IsMatExpr M> auto operator-(const M &m) {
  return expr::MatUnaryOp<M, std::negate<>>(m);
}

template <IsMatExpr M, IsVecExpr V>
  requires(M::columns == V::dim)
auto operator*(const M &m, const V &v) {
  return expr::MatVecMultExpr<M, V>(m, v);
}

template <IsVecExpr V, IsMatExpr M>
  requires(V::dim == M::rows)
auto operator*(const V &v, const M &m) {
  return expr::VecMatMultExpr<V, M>(v, m);
}

template <IsMatExpr L, IsMatExpr R>
  requires(L::columns == R::rows)
auto operator*(const L &l, const R &r) {
  return expr::MatMatMultExpr<L, R>(l, r);
}

template <IsMatExpr M, Scalar S> auto operator*(const M &m, const S &s) {
  using T = typename M::valueType;
  return expr::MatBinaryOp<M, MatScalar<T, M::rows, M::columns>,
                           std::multiplies<>>(
      m, MatScalar<T, M::rows, M::columns>(s));
}

template <IsMatExpr M, Scalar S> auto operator*(const S &s, const M &m) {
  using T = typename M::valueType;
  return expr::MatBinaryOp<MatScalar<T, M::rows, M::columns>, M,
                           std::multiplies<>>(
      MatScalar<T, M::rows, M::columns>(s), m);
}

template <IsMatExpr M>
std::ostream &operator<<(std::ostream &os, const M &mat) {
  for (size_t r = 0; r < M::rows; ++r) {
    os << mat.getRow(r) << (r < M::rows - 1 ? "\n" : "");
  }

  return os;
}

template <IsMatExpr L, IsMatExpr R> auto operator<(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, detail::ComparisonLess>(l, r);
}

template <IsMatExpr L, IsMatExpr R> auto operator>(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, detail::ComparisonGreater>(l, r);
}

template <IsMatExpr L, IsMatExpr R> auto operator<=(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, detail::ComparisonLessEqual>(l, r);
}

template <IsMatExpr L, IsMatExpr R> auto operator>=(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, detail::ComparisonGreaterEqual>(l, r);
}

template <IsMatExpr L, IsMatExpr R> auto operator==(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, detail::ComparisonEqual>(l, r);
}

template <IsMatExpr L, IsMatExpr R> auto operator!=(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, detail::ComparisonNotEqual>(l, r);
}

template <IsMatExpr L, IsMatExpr R> auto operator&&(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, detail::LogicalAnd>(l, r);
}

template <IsMatExpr L, IsMatExpr R> auto operator||(const L &l, const R &r) {
  return expr::MatBinaryOp<L, R, detail::LogicalOr>(l, r);
}

template <IsMatExpr M> auto operator!(const M &m) {
  return expr::MatUnaryOp<M, detail::LogicalNot>(m);
}
} // namespace vect::core