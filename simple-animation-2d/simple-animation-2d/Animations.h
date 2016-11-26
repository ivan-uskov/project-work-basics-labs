#pragma once

#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <assert.h>

enum Direction
{
    Forward  = 1,
    Backward = -1
};

Direction intToDirection(int i)
{
    return Direction(!(i % 2) ? -1 : 1);
}

class Rotor
{
public:
    Rotor(sf::Shape * shape, Direction direction = Forward, float startAngle = 0.f)
        : mShape(shape)
        , mDirection(direction)
    {
        mShape->rotate(mDirection * startAngle);
    }

    void operator () (sf::Time dt)
    {
        mShape->rotate(mDirection * dt.asMilliseconds() * 0.01f);
    }

private:
    Direction mDirection;
    sf::Shape * mShape;
};

class ColorBlinker
{
public:
    ColorBlinker(sf::Shape * shape, Direction direction = Forward, int speed = 1)
        : mShape(shape)
        , mDirection(direction)
        , mSpeed(speed)
    {}

    void operator () (sf::Time dt)
    {
        auto color = mShape->getFillColor();
        color.a += 1;
        mShape->setFillColor(color);
    }

private:
    int mSpeed;
    Direction mDirection;
    sf::Shape * mShape;
};

class Scaler
{
public:
    Scaler(sf::Shape * shape, sf::RenderWindow * window, float coef = 0.005f)
        : mShape(shape)
        , mWindow(window)
        , mCoef(coef)
    {}

    void operator () (sf::Time dt)
    {
        auto pos = mShape->getPosition();
        auto hSize = mWindow->getSize() / 2u;
        auto scale = mShape->getScale();

        scale.x += mCoef * (pos.y >= hSize.y ? 1 : -1);
        scale.y += mCoef * (pos.x <= hSize.x ? 1 : -1);

        mShape->setScale(scale);
    }

private:
    float mCoef;
    sf::Shape * mShape;
    sf::RenderWindow * mWindow;
};

class Mover
{
public:
    Mover(sf::Shape * shape, sf::RenderWindow * window, float speed = 1.f)
        : mShape(shape)
        , mWindow(window)
        , mSpeed(speed)
    {}

    void operator () (sf::Time dt)
    {
        auto pos = mShape->getPosition();
        auto hSize = mWindow->getSize() / 2u;

        auto dx = mSpeed * (pos.y >= hSize.y ? 1 : -1);
        auto dy = mSpeed * (pos.x <= hSize.x ? 1 : -1);

        mShape->move(dx, dy);
    }

private:
    float mSpeed;
    sf::Shape * mShape;
    sf::RenderWindow * mWindow;
};