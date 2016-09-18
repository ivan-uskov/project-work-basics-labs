#pragma once
#include "Config.h"

namespace BalisticMotion
{
    namespace Utils
    {
        template <typename ResultType>
        ResultType getTimeWhenMaxHeightReached(const ResultType& maxHeight)
        {
            return std::sqrt(maxHeight * 2 / BalisticMotion::GRAVITY_AC�ELERATION);
        }

        template <typename ResultType>
        ResultType getInitialOYSpeed(const ResultType& timeWhenMaxHeightReached)
        {
            return timeWhenMaxHeightReached * GRAVITY_AC�ELERATION;
        }
    }
}