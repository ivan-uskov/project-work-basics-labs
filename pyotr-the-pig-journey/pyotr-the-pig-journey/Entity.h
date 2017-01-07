#pragma once

#include "SceneNode.h"

class Entity : public SceneNode
{
public:
    explicit Entity(int hitpoints, Category::Type category = Category::None);

    void setVelocity(sf::Vector2f velocity);
    void setVelocity(float vx, float vy);

    void addCollision(SceneNode*);

    void go(sf::Vector2f const& direction);
    sf::Vector2f getVelocity() const;

    int getHitpoints() const;
    void setHitpoints(int points);
    void repair(int points);
    void damage(int points);
    void destroy();
    virtual void remove();
    virtual bool isDestroyed() const;

protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

private:
    void safeMove(const sf::Vector2f& offset);
    void updateVelocity(float dt);

    std::set<SceneNode*> mCollisions;
    sf::Vector2f mDirection;
    sf::Vector2f mVelocity;
    int mHitpoints;
};
