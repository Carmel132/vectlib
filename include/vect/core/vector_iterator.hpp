#pragma once
#include <iterator>
#include "vect/core/vec_expr_iterator.hpp"
namespace vect::core
{
    template <typename T>
    class VectorIterator
    {
        T *ptr_;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        explicit VectorIterator(T *ptr) : ptr_(ptr) {}

        reference operator*() const { return *ptr_; }
        VectorIterator &operator++()
        {
            ++ptr_;
            return *this;
        }
        VectorIterator &operator--()
        {
            --ptr_;
            return *this;
        }
        VectorIterator operator+(difference_type n) const { return VectorIterator(ptr_ + n); }
        difference_type operator-(const VectorIterator &other) const { return ptr_ - other.ptr_; }

        bool operator==(const VectorIterator &other) const = default;
        auto operator<=>(const VectorIterator &other) const = default;
    };
}

namespace std
{
    template <typename E>
    struct iterator_traits<vect::core::ExprIterator<E>>
    {
        using difference_type = std::ptrdiff_t;
        using value_type = typename E::valueType;
        using pointer = const value_type *;
        using reference = value_type;
        using iterator_category = std::input_iterator_tag;
    };
}