#include "stdafx.h"
#include "Entity.h"
#include "Utility.h"

#include <cassert>

namespace
{
    const float GRAVITY = 9.8f;
    const float TRACTION = 10.f;
    const float FRICTION = 3.f;
    const float PIXELS_IN_METER = 30;
    const auto MAX_VELOCITY = sf::Vector2f(800, 400);
}

Entity::Entity(int hitpoints, Category::Type category)
    : SceneNode(category)
    , mHitpoints(hitpoints)
{}

void Entity::reset()
{
    mVelocity = sf::Vector2f();
    mDirection = sf::Vector2f();
    mCollisions.clear();
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

void Entity::addCollision(SceneNode* node)
{
    mCollisions.insert(node);
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

void Entity::doDestroy()
{
    mHitpoints = 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
    auto dts = dt.asSeconds();
    updateVelocity(dts);
    safeMove(mVelocity * dts);

    mDirection = sf::Vector2f();
    mCollisions.clear();
}

void Entity::safeMove(const sf::Vector2f& offset)
{
    for (auto node : mCollisions)
    {
        auto pos = getPosition();
        auto rect = getBoundingRect();
        auto nRect = node->getBoundingRect();

        pos.y = std::min(pos.y, nRect.top - rect.height / 2);
        if (mVelocity.y >= 0)
        {
            move(pos - getPosition());
        }
    }

    move(offset);
}

void Entity::updateVelocity(float dt)
{
    auto tractionAcceleration = (normalize(mDirection) * TRACTION);
    auto gravityAcceleration = GRAVITY * sf::Vector2f(0, 1);
    auto frictionAcceleration = mCollisions.empty() ? sf::Vector2f() : normalize(sf::Vector2f(-mVelocity.x, 0)) * FRICTION;
    auto resultAccseleration =
        tractionAcceleration +
        gravityAcceleration +
        frictionAcceleration;

    resultAccseleration *= PIXELS_IN_METER;
    mVelocity += resultAccseleration * dt;
    mVelocity = min(mVelocity, MAX_VELOCITY);
    mVelocity = max(mVelocity, -MAX_VELOCITY);
}
