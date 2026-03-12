// #include <array>
#include <print>
#include <vect/vector.hpp>

template <typename T>
void printVector(const vect::core::VecExpr<T> &vec)
{
    for (size_t idx{}; idx < vec.size(); ++idx)
    {
        std::print("{}, ", vec[idx]);
    }
}

int main()
{
    vect::Vector<float, 7> v1{1, 2, 3};
    vect::Vector<float, 4> v2{1, 0 ,3};

    //vect::Vector<double, 3> v3 = v1 * 5.;
    
    //auto d = vect::dot(v1, v2);

    //auto v3 = vect::cross(vect::Vector<double, 3>{1, 2}, vect::Vector<double, 3>{1, 0});
    //v1 = v1 + v2;
    v1 = -v1;
    //std::print("Should be equal: {}, should not (different): {}, should not (size): {}", v1==v2, vect::sin(v1) == v2, v1.swizzle<2, 0>() == v2);
    //std::println("Sum: {}", );
    std::cout << v1 << "\n" << -v2;
    return 0;
}