#pragma once
#include "vect/expr/mat_operator.hpp"
#include <type_traits>
namespace vect::detail {

template <typename T> struct MatDeferAssign : std::false_type {};
template <typename L, typename R>
struct MatDeferAssign<expr::MatMatMultExpr<L, R>> : std::true_type {};

} // namespace vect::detail