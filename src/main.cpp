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

  auto res = vect::expr::MatSlice<decltype(sq), 1>(sq);
  vect::Vector<float, 3> v{0, 1, 2};
  printMat(sq);
  std::cout << "-----------\n";
  printMat(res);
  std::cout << res << "\n";
  std::cout << "\n-----------instance\n" << sq.slice<1, 3, 1, 3>();

  return 0;
}