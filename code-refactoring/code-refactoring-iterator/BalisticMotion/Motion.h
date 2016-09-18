#pragma once
#include "MotionUtils.h"

namespace BalisticMotion
{
    template <typename ResultType>
    class Motion
    {
    public:
        typedef typename ResultType ValueType;

        Motion(const ResultType & maxHeight)
            : m_maxHeight(maxHeight)
        {
            m_timeWhenMaxHeightReached = Utils::getTimeWhenMaxHeightReached(maxHeight);
            m_initialOYSpeed = Utils::getInitialOYSpeed(m_timeWhenMaxHeightReached);
        }

        ResultType getMaxHeight() const
        {
            return m_maxHeight;
        }

        ResultType getTimeWhenMaxHeightReached() const
        {
            return m_timeWhenMaxHeightReached;
        }

        ResultType getMotionEndTime() const
        {
            return m_timeWhenMaxHeightReached * 2;
        }

        ResultType getHeightForTime(const ResultType& time)
        {
            return m_initialOYSpeed * time - 0.5f * GRAVITY_ACÑELERATION * time * time;
        }

    private:
        ResultType m_maxHeight;
        ResultType m_initialOYSpeed;
        ResultType m_timeWhenMaxHeightReached;
    };
}