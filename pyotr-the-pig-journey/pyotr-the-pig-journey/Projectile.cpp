#include "stdafx.h"
#include "Projectile.h"

#include "EmitterNode.h"
#include "DataTables.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>

namespace
{
    const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures)
    : Entity(1, Category::AlliedProjectile)
    , mType(type)
    , mSprite(textures.get(Table[type].texture), Table[type].textureRect)
    , mTargetDirection()
{
    centerOrigin(mSprite);

    // Add particle system for missiles
    if (isGuided())
    {
        std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke));
        smoke->setPosition(-getBoundingRect().width / 2.f, 0.f);
        attachChild(std::move(smoke));

        std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant));
        propellant->setPosition(-getBoundingRect().width / 2.f, 0.f);
        attachChild(std::move(propellant));

    }
}

void Projectile::guideTowards(sf::Vector2f position)
{
    assert(isGuided());
    mTargetDirection = normalize(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
    return mType == Missile;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (isGuided())
    {
        const float approachRate = 200.f;

        sf::Vector2f newVelocity = normalize(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
        newVelocity *= getMaxSpeed();
        float angle = std::atan2(newVelocity.y, newVelocity.x);

        setRotation(toDegree(angle));
        setVelocity(newVelocity);
    }

    Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
    return Table[mType].speed;
}

int Projectile::getDamage() const
{
    return Table[mType].damage;
}
