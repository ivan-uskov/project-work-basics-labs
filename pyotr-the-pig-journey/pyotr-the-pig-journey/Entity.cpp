#include "stdafx.h"
#include "Entity.h"
#include "Utility.h"

#include <cassert>

namespace
{
    const float GRAVITY = 9.8f;
    const float TRACTION = 10.f;
    const float PIXELS_IN_METER = 30;
}

Entity::Entity(int hitpoints, Category::Type category)
    : SceneNode(category)
    , mHitpoints(hitpoints)
{}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
    return mVelocity;
}

void Entity::go(sf::Vector2f const& direction)
{
    mDirection += direction;
}

int Entity::getHitpoints() const
{
    return mHitpoints;
}

void Entity::setHitpoints(int points)
{
    assert(points > 0);
    mHitpoints = points;
}

void Entity::repair(int points)
{
    assert(points > 0);

    mHitpoints += points;
}

void Entity::damage(int points)
{
    assert(points > 0);

    mHitpoints -= points;
}

void Entity::destroy()
{
    mHitpoints = 0;
}

void Entity::remove()
{
    destroy();
}

bool Entity::isDestroyed() const
{
    return mHitpoints <= 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
    auto resultAccseleration = (normalize(mDirection) * TRACTION) + GRAVITY * sf::Vector2f(0, 1);
    resultAccseleration *= PIXELS_IN_METER;
    mVelocity += resultAccseleration * dt.asSeconds();
    move(mVelocity * dt.asSeconds());

    mDirection = sf::Vector2f(0, 0);
}
