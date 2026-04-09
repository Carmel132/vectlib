#pragma once

#include "vect/core/mat_expr.hpp"
#include "vect/core/scalar.hpp"
#include "vect/core/vec_expr.hpp"
#include "vect/expr/mat_column_view.hpp"
#include "vect/expr/mat_unary_op.hpp"
namespace vect::detail {
template <size_t R, typename M, typename V, typename Op>
auto column_reduce_helper(const M &mat, const V &acc, Op op) {
  if constexpr (R == M::rows) {
    return acc;
  } else {
    return column_reduce_helper<R + 1>(
        mat, expr::BinaryOp(acc, mat.getRow(R), op), op);
  }
}
} // namespace vect::detail

namespace vect::expr {

template <typename M>
class TransposeExpr
    : public core::MatExpr<TransposeExpr<M>, typename M::valueType, M::columns,
                           M::rows> {
  core::capture_t<M> m_;

public:
  TransposeExpr(const M &m) : m_{m} {}

  [[nodiscard]] auto at(size_t r, size_t c) const { return m_.at(c, r); }

  [[nodiscard]] auto getRow(size_t r) const { return MatColumnView<M>(m_, r); }
};

template <core::IsMatExpr M> auto transpose(const M &m) {
  return TransposeExpr<M>(m);
}

template <core::IsMatExpr M, typename F> constexpr auto map(M &&m, F &&f) {
  using ExprType = std::decay_t<M>;
  using FuncType = std::decay_t<F>;

  return MatUnaryOp<ExprType, FuncType>(std::forward<M>(m), std::forward<F>(f));
}

template <typename T>
concept IsMappableExpr = core::IsMatExpr<T> || core::IsVecExpr<T>;

template <IsMappableExpr V, core::Scalar S>
constexpr auto max(const V &v, S s) {
  return map(v, [s](auto x) { return x > s ? x : s; });
}

template <IsMappableExpr V, core::Scalar S>
constexpr auto min(const V &v, S s) {
  return map(v, [s](auto x) { return x < s ? x : s; });
}

template <IsMappableExpr V> constexpr auto abs(const V &v) {
  return map(v, [](auto x) {
    using std::abs;
    return abs(x);
  });
}

template <IsMappableExpr V> constexpr auto sqrt(const V &v) {
  return map(v, [](auto x) {
    using std::sqrt;
    return sqrt(x);
  });
}

template <IsMappableExpr V> constexpr auto exp(const V &v) {
  return map(v, [](auto x) {
    using std::exp;
    return exp(x);
  });
}

template <IsMappableExpr V> constexpr auto log(const V &v) {
  return map(v, [](auto x) {
    using std::log;
    return log(x);
  });
}

template <IsMappableExpr V> constexpr auto negate(const V &v) {
  return map(v, [](auto x) { return -x; });
}

template <IsMappableExpr V> constexpr auto sin(const V &v) {
  return map(v, [](auto x) {
    using std::sin;
    return sin(x);
  });
}

template <IsMappableExpr V> constexpr auto cos(const V &v) {
  return map(v, [](auto x) {
    using std::cos;
    return cos(x);
  });
}

template <IsMappableExpr V> constexpr auto tan(const V &v) {
  return map(v, [](auto x) {
    using std::tan;
    return tan(x);
  });
}

template <IsMappableExpr V> constexpr auto asin(const V &v) {
  return map(v, [](auto x) {
    using std::asin;
    return asin(x);
  });
}

template <IsMappableExpr V> constexpr auto acos(const V &v) {
  return map(v, [](auto x) {
    using std::acos;
    return acos(x);
  });
}

template <IsMappableExpr V> constexpr auto atan(const V &v) {
  return map(v, [](auto x) {
    using std::atan;
    return atan(x);
  });
}

template <IsMappableExpr V> constexpr auto floor(const V &v) {
  return map(v, [](auto x) {
    using std::floor;
    return floor(x);
  });
}

template <IsMappableExpr V> constexpr auto ceil(const V &v) {
  return map(v, [](auto x) {
    using std::ceil;
    return ceil(x);
  });
}

template <IsMappableExpr V> constexpr auto round(const V &v) {
  return map(v, [](auto x) {
    using std::round;
    return round(x);
  });
}

template <IsMappableExpr V> constexpr auto sign(const V &v) {
  return map(v, [](auto x) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); });
}

template <IsMappableExpr V> constexpr auto fract(const V &v) {
  return map(v, [](auto x) {
    using std::floor;
    return x - floor(x);
  });
}

template <IsMappableExpr V, core::Scalar S>
constexpr auto clamp(const V &v, S lo, S hi) {
  return map(v, [lo, hi](auto x) {
    return std::clamp(x, static_cast<decltype(x)>(lo),
                      static_cast<decltype(x)>(hi));
  });
}

template <core::IsMatExpr M, core::IsVecExpr V, typename Op>
auto reduceColumns(const M &mat, const V &identity, Op op = Op{}) {
  return detail::column_reduce_helper<0>(mat, identity.self(), op);
}

template <core::IsMatExpr M, typename Op>
auto reduceColumns(const M &mat, typename M::valueType identity, Op op = Op{}) {
  return reduceColumns(
      mat, core::VecScalar<typename M::valueType, M::columns>(identity), op);
}

template <typename M, typename V, typename Op>
class MatRowReduceExpr : public core::VecExpr<MatRowReduceExpr<M, V, Op>> {
  core::capture_t<M> m_;
  Op op_;
  core::capture_t<V> identity_;

public:
  static constexpr size_t dim = M::rows;
  using valueType = typename M::valueType;

  MatRowReduceExpr(const M &m, const V &identity, Op op = Op{})
      : m_(m), op_(op), identity_(identity) {}

  [[nodiscard]] valueType operator[](size_t idx) const {
    return m_.getRow(idx).reduce(op_, identity_[idx]);
  }

  [[nodiscard]] size_t size() const { return dim; }
};

template <core::IsMatExpr M, core::IsVecExpr V, typename Op>
auto reduceRows(const M &mat, const V &identity, Op op = Op{}) {
  return MatRowReduceExpr<M, V, Op>(mat, identity, op);
}

template <core::IsMatExpr M, typename Op>
auto reduceRows(const M &mat, typename M::valueType identity, Op op = Op{}) {
  return reduceRows(
      mat, core::VecScalar<typename M::valueType, M::rows>(identity), op);
}

template <core::IsMatExpr M> auto rowMin(const M &mat) {
  using std::min;
  return reduceRows(mat, std::numeric_limits<typename M::valueType>::max(),
                    [](auto a, auto b) { return min(a, b); });
}

template <core::IsMatExpr M> auto rowMax(const M &mat) {
  using std::max;
  return reduceRows(mat, std::numeric_limits<typename M::valueType>::lowest(),
                    [](auto a, auto b) { return max(a, b); });
}

template <core::IsMatExpr M> auto rowSum(const M &mat) {
  return reduceRows(mat, typename M::valueType(0), std::plus<>());
}

template <core::IsMatExpr M> auto columnSum(const M &mat) {
  return reduceColumns(mat, typename M::valueType(0), std::plus<>());
}

template <core::IsMatExpr M> auto columnMin(const M &mat) {
  using std::min;
  return reduceColumns(mat, std::numeric_limits<typename M::valueType>::max(),
                       [](auto a, auto b) { return min(a, b); });
}

template <core::IsMatExpr M> auto columnMax(const M &mat) {
  using std::max;
  return reduceColumns(mat,
                       std::numeric_limits<typename M::valueType>::lowest(),
                       [](auto a, auto b) { return max(a, b); });
}

template <core::IsMatExpr M> auto columnMean(const M &mat) {
  return reduceColumns(mat, typename M::valueType(0), std::plus<>()) /
         static_cast<typename M::valueType>(M::rows);
}

template <core::IsMatExpr M> auto rowMean(const M &mat) {
  return reduceRows(mat, typename M::valueType(0), std::plus<>()) /
         static_cast<typename M::valueType>(M::columns);
}

template <core::IsMatExpr M> auto rowProduct(const M &mat) {
  return reduceRows(mat, typename M::valueType(1), std::multiplies<>());
}

template <core::IsMatExpr M> auto columnProduct(const M &mat) {
  return reduceColumns(mat, typename M::valueType(1), std::multiplies<>());
}

template <typename M> class MatDiagView : public core::VecExpr<MatDiagView<M>> {
  core::capture_t<M> m_;

public:
  using valueType = typename M::valueType;
  static constexpr size_t dim = M::rows > M::columns ? M::columns : M::rows;

  MatDiagView(const M &m) : m_(m) {}

  [[nodiscard]] constexpr valueType operator[](size_t idx) const {
    return m_.at(idx, idx);
  }

  [[nodiscard]] constexpr auto size() const -> size_t { return dim; }

  [[nodiscard]] auto loadPacket(size_t idx) const {
    return loadPacketUnaligned(idx);
  }

  [[nodiscard]] auto loadPacketUnaligned(size_t idx) const {
    using Traits = detail::SimdTraits<valueType, dim>;
    using Packet = typename Traits::packetType;
    // TODO: AVX optimization?
    alignas(Traits::alignment) valueType vals[Traits::width];
    for (size_t i = 0; i < Traits::width; ++i) {
      if (idx + i < dim) {
        vals[i] = m_.at(idx + i, idx + i);
      } else {
        vals[i] = valueType{0};
      }
    }
    return detail::load<Packet, Traits>(vals);
  }
};
template <core::IsMatExpr M> auto diag(const M &mat) {
  return MatDiagView(mat);
}

} // namespace vect::expr