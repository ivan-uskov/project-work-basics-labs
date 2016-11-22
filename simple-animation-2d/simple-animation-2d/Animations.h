#pragma once

#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <assert.h>

class Rotor
{
public:
    Rotor(std::shared_ptr<sf::Shape> const& shape)
        : mShape(shape)
    {}

    void operator () (sf::Time dt)
    {
        mStartAngle += dt.asMilliseconds();
        if (mStartAngle > 360)
        {
            mStartAngle -= floor(mStartAngle / 360) * 360;
        }

        assert(mStartAngle <= 360);

        mShape->setPosition(mStartAngle, mStartAngle);
    }

private:
    float mStartAngle = 0.f;
    std::shared_ptr<sf::Shape> const& mShape;
};