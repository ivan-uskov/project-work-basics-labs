#pragma once

namespace Math
{
    template <typename ValueType>
    bool isFloatPointNumbersEquals(const ValueType& lhs, const ValueType& rhs)
    {
        return std::fabs(lhs - rhs) < DBL_EPSILON;
    }
}