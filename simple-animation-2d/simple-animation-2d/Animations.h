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

class ColorBlinker
{
public:
    ColorBlinker(sf::Shape * shape)
        : mShape(shape)
    {}

    void operator () (sf::Time dt)
    {
        auto color = mShape->getFillColor();
        color.a += 1;
        mShape->setFillColor(color);
    }

private:
    sf::Shape * mShape;
};

class Scaler
{
public:
    Scaler(sf::Shape * shape, sf::RenderWindow * window)
        : mShape(shape)
        , mWindow(window)
    {}

    void operator () (sf::Time dt)
    {
        auto color = mShape->getFillColor();
        color.a += 1;
        mShape->setFillColor(color);
    }

private:
    sf::Shape * mShape;
    sf::RenderWindow * mWindow;
};