#pragma once
#include <iterator>
namespace vect::core
{

    template <typename T>
    class ExprIterator
    {
        const T *expr_;
        size_t idx_;

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = typename T::valueType;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type *;
        using reference = value_type;

        ExprIterator(const T &e, size_t i) : expr_(&e), idx_(i) {}

        reference operator*() const { return (*expr_)[idx_]; }
        ExprIterator &operator++()
        {
            ++idx_;
            return *this;
        }

        bool operator==(const ExprIterator &other) const { return idx_ == other.idx_; }
        bool operator!=(const ExprIterator &other) const { return !(*this == other); }
    };

}