#pragma once
#include <type_traits>

namespace vect::expr {
template <typename L, typename R> class MatMatMultExpr;
template <typename L, typename R> class VecMatMultExpr;
}

namespace vect::detail {

template <typename T> struct DeferAssign : std::false_type {};
template <typename L, typename R>
struct DeferAssign<expr::MatMatMultExpr<L, R>> : std::true_type {};
template <typename L, typename R>
struct DeferAssign<expr::VecMatMultExpr<L, R>> : std::true_type {};
} // namespace vect::detail