#pragma once
#include "vect/core/vector.hpp"
namespace vect::expr
{
    template <typename L, typename R>
    class VecAdd : public vect::core::VecExpr<VecAdd<L, R>>
    {
        const L &l_;
        const R &r_;

    public:
        VecAdd(const L &l, const R &r) : l_{l}, r_{r} {}

        auto operator[](size_t idx) const
        {
            return l_[idx] + r_[idx];
        }

        [[nodiscard]] auto size() const -> size_t
        {
            return l_.size();
        }
    };

    template <typename L, typename R>
    class VecSub : public vect::core::VecExpr<VecSub<L, R>>
    {
        const L &l_;
        const R &r_;

    public:
        VecSub(const L &l, const R &r) : l_{l}, r_{r} {}

        auto operator[](size_t idx) const
        {
            return l_[idx] - r_[idx];
        }

        [[nodiscard]] auto size() const -> size_t
        {
            return l_.size();
        }
    };
}
namespace vect
{
    template <typename L, typename R>
    auto operator+(const vect::core::VecExpr<L> &l, const vect::core::VecExpr<R> &r)
    {
        return expr::VecAdd<L, R>(l.self(), r.self());
    }
}