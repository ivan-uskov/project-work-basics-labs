#pragma once

#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <assert.h>

class Rotor
{
public:
    Rotor(sf::Shape * shape)
        : mShape(shape)
    {}

    void operator () (sf::Time dt)
    {
        mShape->rotate(dt.asMilliseconds() * 0.01f);
    }

private:
    sf::Shape * mShape;
};