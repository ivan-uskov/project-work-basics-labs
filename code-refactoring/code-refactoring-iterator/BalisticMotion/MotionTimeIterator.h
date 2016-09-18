#pragma once
#include "Motion.h"
#include "Math.h"

namespace BalisticMotion
{
    template <typename MotionType>
    class MotionTimeIterator : public std::iterator<std::forward_iterator_tag, typename MotionType::ValueType>
    {
    public:
        typedef typename MotionType::ValueType ValueType;

        MotionTimeIterator()
            : m_isEnd(true)
        {}

        MotionTimeIterator(std::shared_ptr<MotionType> & motion, const ValueType & timeOffset)
            : m_motion(motion)
            , m_timeOffset(timeOffset)
        {}

        MotionTimeIterator& operator ++ ()
        {
            inc();
            return *this;
        }

        MotionTimeIterator operator ++ (int)
        {
            MotionTimeIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        bool operator == (const MotionTimeIterator& rhs)
        {
            return (m_isEnd && rhs.m_isEnd) || (
                Math::isFloatPointNumbersEquals(m_currentValue, rhs.m_currentValue) &&
                Math::isFloatPointNumbersEquals(m_timeOffset, rhs.m_timeOffset) &&
                m_motion.get() == rhs.m_motion.get()
            );
        }

        bool operator != (const MotionTimeIterator& rhs)
        {
            return !(*this == rhs);
        }

        auto operator * ()
        {
            return m_currentValue;
        }

    private:
        bool m_isEnd = false;
        std::shared_ptr<MotionType> m_motion;
        std::shared_ptr<ValueType> m_savedValue;
        ValueType m_currentValue = 0;
        ValueType m_timeOffset;

        void inc()
        {
            auto newVal = getNewValue();
            auto maxHeightTime = m_motion->getTimeWhenMaxHeightReached();
            auto motionEndTime = m_motion->getMotionEndTime();

            if (isCurrentValueLast())
            {
                m_isEnd = true;
            }
            else if ((m_currentValue < maxHeightTime) && (maxHeightTime < newVal))
            {
                saveValue();
                m_currentValue = maxHeightTime;
            }
            else if ((m_currentValue < motionEndTime) && (motionEndTime < newVal))
            {
                m_currentValue = motionEndTime;
            }
            else
            {
                m_currentValue = newVal;
            }
        }

        bool isCurrentValueLast() const
        {
            return Math::isFloatPointNumbersEquals(m_currentValue, m_motion->getMotionEndTime());
        }

        ValueType getNewValue()
        {
            ValueType newValue;

            if (m_savedValue)
            {
                newValue = *m_savedValue + m_timeOffset;
                m_savedValue.reset();
            }
            else
            {
                newValue = m_currentValue + m_timeOffset;
            }

            return newValue;
        }

        void saveValue()
        {
            m_savedValue = std::make_shared<ValueType>(m_currentValue);
        }
    };
}
