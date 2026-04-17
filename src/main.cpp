// #include <array>
#include "vect/core/matrix.hpp"
#include "vect/expr/mat_binary_op.hpp"
#include "vect/expr/mat_method.hpp"
#include "vect/expr/mat_operator.hpp"
#include "vect/expr/mat_slice.hpp"
#include "vect/expr/vector_operator.hpp"
#include <cstddef>
#include <functional>
#include <print>
#include <vect/vector.hpp>
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

  auto tst = vect::core::Matrix<float, 3, 3>{1, -2, 3, -4, 5, -6, 7, -8, 9};

  std::cout << tst << "\n" << sq << "\n" << (tst < sq) << "\n";
  std::cout << vect::expr::where(tst < sq, tst, sq);
  std::cout << "\n\n";
  printMat((tst < sq));
  return 0;
}