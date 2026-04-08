// #include <array>
#include "vect/core/matrix.hpp"
#include "vect/expr/mat_binary_op.hpp"
#include "vect/expr/mat_method.hpp"
#include "vect/expr/mat_operator.hpp"
#include <cstddef>
#include <print>
#include <vect/vector.hpp>
#include <functional>
template <typename T> void printMat(T mat) {
  for (size_t r = 0; r < T::rows; ++r) {
    for (size_t c = 0; c < T::columns; ++c) {
      std::cout << mat.at(r, c) << " ";
    }
    std::cout << "\n";
  }
}

auto main() -> int {

  auto sq = vect::core::Matrix<float, 3, 3>{1, 2, 3, 4, 5, 6, 7, 8, 9};

  auto res = vect::expr::reduceRows<decltype(sq), vect::Vector<float, 3>, std::plus<> >(sq, {0, 0, 0});
  auto res2 = vect::expr::reduceRows<decltype(sq), std::plus<>>(sq, 0);
  printMat(sq );
  std::cout << "\n" << res<<"\n"<<res2<<"\n";
  return 0;
}