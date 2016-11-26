#include "stdafx.h"
#include "Tractor.h"

#include "DataTables.h"
#include "Utility.h"
#include "Pickup.h"
#include "CommandQueue.h"
#include "SoundNode.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

using namespace std::placeholders;

namespace
{
    const std::vector<TractorData> Table = initializeTractorData();
}

Tractor::Tractor(Type type, const TextureHolder& textures, const FontHolder& fonts)
    : Entity(Table[type].hitpoints)
    , mType(type)
    , mSprite(textures.get(Table[type].texture), Table[type].textureRect)
    , mExplosion(textures.get(Textures::Explosion))
{
    mExplosion.setFrameSize(sf::Vector2i(256, 256));
    mExplosion.setNumFrames(16);
    mExplosion.setDuration(sf::seconds(1));

    centerOrigin(mSprite);
    centerOrigin(mExplosion);

    mFireCommand.category = Category::SceneAirLayer;
    mFireCommand.action = [this, &textures](SceneNode& node, sf::Time) {
        createBullets(node, textures);
    };

    mMissileCommand.category = Category::SceneAirLayer;
    mMissileCommand.action = [this, &textures](SceneNode& node, sf::Time) {
        createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
    };

    mDropPickupCommand.category = Category::SceneAirLayer;
    mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time)
    {
        createPickup(node, textures);
    };

    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
    healthDisplay->setPosition(0.f, -70.f);
    mHealthDisplay = healthDisplay.get();
    attachChild(std::move(healthDisplay));

    if (getCategory() == Category::PlayerTractor)
    {
        std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
        missileDisplay->setPosition(0, -50.f);
        mMissileDisplay = missileDisplay.get();
        attachChild(std::move(missileDisplay));
    }

    updateTexts();
}

int Tractor::getMissileAmmo() const
{
    return mMissileAmmo;
}

void Tractor::setMissileAmmo(int ammo)
{
    mMissileAmmo = ammo;
}

void Tractor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (isDestroyed() && mShowExplosion)
    {
        target.draw(mExplosion, states);
    }
    else
    {
        target.draw(mSprite, states);
    }
}

void Tractor::disablePickups()
{
    mPickupsEnabled = false;
}

void Tractor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    updateTexts();
    updateRollAnimation();

    if (isDestroyed())
    {
        checkPickupDrop(commands);
        mExplosion.update(dt);

        if (!mExplosionBegan)
        {
            SoundEffect::ID soundEffect = (randomInt(2) == 0) ? SoundEffect::Explosion1 : SoundEffect::Explosion2;
            playLocalSound(commands, soundEffect);

            mExplosionBegan = true;
        }
        return;
    }

    checkProjectileLaunch(dt, commands);
    Entity::updateCurrent(dt, commands);
}

unsigned int Tractor::getCategory() const
{
    if (isAllied())
        return Category::PlayerTractor;
    else
        return Category::EnemyTractor;
}

sf::FloatRect Tractor::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Tractor::isMarkedForRemoval() const
{
    return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

void Tractor::remove()
{
    Entity::remove();
    mShowExplosion = false;
}

bool Tractor::isAllied() const
{
    return mType == Eagle;
}

float Tractor::getMaxSpeed() const
{
    return Table[mType].speed;
}

void Tractor::increaseFireRate()
{
    if (mFireRateLevel < 10)
        ++mFireRateLevel;
}

void Tractor::increaseSpread()
{
    if (mSpreadLevel < 3)
        ++mSpreadLevel;
}

void Tractor::collectMissiles(unsigned int count)
{
    mMissileAmmo += count;
}

void Tractor::fire()
{
    // Only ships with fire interval != 0 are able to fire
    if (Table[mType].fireInterval != sf::Time::Zero)
        mIsFiring = true;
}

void Tractor::launchMissile()
{
    if (mMissileAmmo > 0)
    {
        mIsLaunchingMissile = true;
        --mMissileAmmo;
    }
}

void Tractor::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
    sf::Vector2f worldPosition = getWorldPosition();

    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode>(
        [effect, worldPosition](SoundNode& node, sf::Time)
    {
        node.playSound(effect, worldPosition);
    });

    commands.push(command);
}

int	Tractor::getIdentifier()
{
    return mIdentifier;
}

void Tractor::setIdentifier(int identifier)
{
    mIdentifier = identifier;
}

void Tractor::checkPickupDrop(CommandQueue& commands)
{
    // Drop pickup, if enemy airplane, with probability 1/3, if pickup not yet dropped
    // and if not in network mode (where pickups are dropped via packets)
    if (!isAllied() && randomInt(3) == 0 && !mSpawnedPickup && mPickupsEnabled)
        commands.push(mDropPickupCommand);

    mSpawnedPickup = true;
}

void Tractor::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
    // Enemies try to fire all the time
    if (!isAllied())
        fire();

    // Check for automatic gunfire, allow only in intervals
    if (mIsFiring && mFireCountdown <= sf::Time::Zero)
    {
        // Interval expired: We can fire a new bullet
        commands.push(mFireCommand);
        playLocalSound(commands, isAllied() ? SoundEffect::AlliedGunfire : SoundEffect::EnemyGunfire);

        mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
        mIsFiring = false;
    }
    else if (mFireCountdown > sf::Time::Zero)
    {
        // Interval not expired: Decrease it further
        mFireCountdown -= dt;
        mIsFiring = false;
    }

    // Check for missile launch
    if (mIsLaunchingMissile)
    {
        commands.push(mMissileCommand);
        playLocalSound(commands, SoundEffect::LaunchMissile);

        mIsLaunchingMissile = false;
    }
}

void Tractor::createBullets(SceneNode& node, const TextureHolder& textures) const
{
    Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

    switch (mSpreadLevel)
    {
    case 1:
        createProjectile(node, type, 0.0f, 0.5f, textures);
        break;

    case 2:
        createProjectile(node, type, -0.33f, 0.33f, textures);
        createProjectile(node, type, +0.33f, 0.33f, textures);
        break;

    case 3:
        createProjectile(node, type, -0.5f, 0.33f, textures);
        createProjectile(node, type, 0.0f, 0.5f, textures);
        createProjectile(node, type, +0.5f, 0.33f, textures);
        break;
    }
}

void Tractor::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

    sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
    sf::Vector2f velocity(0, projectile->getMaxSpeed());

    float sign = isAllied() ? -1.f : +1.f;
    projectile->setPosition(getWorldPosition() + offset * sign);
    projectile->setVelocity(velocity * sign);
    node.attachChild(std::move(projectile));
}

void Tractor::createPickup(SceneNode& node, const TextureHolder& textures) const
{
    auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

    std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
    pickup->setPosition(getWorldPosition());
    pickup->setVelocity(0.f, 1.f);
    node.attachChild(std::move(pickup));
}

void Tractor::updateTexts()
{
    mHealthDisplay->setString(isDestroyed() ? "" : toString(getHitpoints()) + " HP");
    mHealthDisplay->setRotation(-getRotation());

    // Display missiles, if available
    if (mMissileDisplay)
    {
        if (mMissileAmmo == 0 || isDestroyed())
        {
            mMissileDisplay->setString("");
        }
        else
        {
            mMissileDisplay->setString("R: " + toString(mMissileAmmo));
        }
    }
}

void Tractor::updateRollAnimation()
{
    if (Table[mType].hasRollAnimation)
    {
        sf::IntRect textureRect = Table[mType].textureRect;

        // Roll left: Texture rect offset once
        if (getVelocity().x < 0.f)
        {
            textureRect.left += textureRect.width;
        }
        else if (getVelocity().x > 0.f)
        {
            // Roll right: Texture rect offset twice
            textureRect.left += 2 * textureRect.width;
        }

        mSprite.setTextureRect(textureRect);
    }
}
