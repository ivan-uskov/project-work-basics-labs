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
#include <SFML/Graphics/CircleShape.hpp>

#include <cmath>

using namespace std::placeholders;

namespace
{
    const std::vector<TractorData> Table = initializeTractorData();
    const sf::Vector2f SMALL_WHEEL_POS = { 55, 62 };
    const sf::Vector2f BIG_WHEEL_POS = { -10, 50 };
}

Tractor::Tractor(Type type, const TextureHolder& textures, const FontHolder& fonts)
    : Entity(Table[type].hitpoints, Category::Tractor)
    , mType(type)
    , mSprite(textures.get(Table[type].texture), Table[type].textureRect)
    , mBigWheelSprite(textures.get(Table[type].texture), Table[type].bigWheelTextureRect)
    , mSmallWheelSprite(textures.get(Table[type].texture), Table[type].smallWheelTextureRect)
    , mExplosion(textures.get(Textures::Explosion))
{
    mExplosion.setFrameSize(sf::Vector2i(256, 256));
    mExplosion.setNumFrames(16);
    mExplosion.setDuration(sf::seconds(1));

    centerOrigin(mSprite);
    centerOrigin(mBigWheelSprite);
    centerOrigin(mSmallWheelSprite);
    centerOrigin(mExplosion);

    mSmallWheelSprite.setPosition(SMALL_WHEEL_POS);
    mBigWheelSprite.setPosition(BIG_WHEEL_POS);

    mFireCommand.category = Category::SceneAirLayer;
    mFireCommand.action = [this, &textures](SceneNode& node, sf::Time) {
        createBullets(node, textures);
    };

    mMissileCommand.category = Category::SceneAirLayer;
    mMissileCommand.action = [this, &textures](SceneNode& node, sf::Time) {
        createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
    };

    auto healthDisplay = std::make_unique<TextNode>(fonts, "");
    healthDisplay->setPosition(0.f, -90.f);
    mHealthDisplay = healthDisplay.get();
    attachChild(std::move(healthDisplay));

    auto missileDisplay = std::make_unique<TextNode>(fonts, "");
    missileDisplay->setPosition(0, -70.f);
    mMissileDisplay = missileDisplay.get();
    attachChild(std::move(missileDisplay));

    updateTexts();

    {
        auto pos = getPosition();
        auto rect = getBoundingRect();

        auto dx = pos.x - (rect.left + rect.width / 2.f);
        auto dy = pos.y - (rect.top + rect.height / 2.f);

        mSprite.move(dx, dy);
        mBigWheelSprite.move(dx, dy);
        mSmallWheelSprite.move(dx, dy);
        mExplosion.move(dx, dy);
        mHealthDisplay->move(dx, dy);
        mMissileDisplay->move(dx, dy);
    }
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
        target.draw(mBigWheelSprite, states);
        target.draw(mSmallWheelSprite, states);
    }
}

void Tractor::disablePickups()
{
    mPickupsEnabled = false;
}

void Tractor::updateWheelsRotation(sf::Time dt)
{
    mBigWheelSprite.rotate(getVelocity().x * dt.asSeconds());
    mSmallWheelSprite.rotate(getVelocity().x * dt.asSeconds());
}

void Tractor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    updateTexts();
    updateRollAnimation();
    updateWheelsRotation(dt);

    if (isDestroyed())
    {
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

sf::FloatRect Tractor::getBoundingRect() const
{
    auto right = [](auto & rect) {
        return rect.left + rect.width;
    };

    auto bottom = [](auto & rect) {
        return rect.top + rect.height;
    };

    auto getBounds = [](sf::Sprite const& sprite) {
        auto baseBounds = sprite.getLocalBounds();
        baseBounds.left = sprite.getPosition().x - baseBounds.width / 2.f;
        baseBounds.top = sprite.getPosition().y - baseBounds.height / 2.f;

        return baseBounds;
    };

    auto baseBounds = getBounds(mSprite);
    auto smallBounds = getBounds(mSmallWheelSprite);
    auto bigBounds = getBounds(mBigWheelSprite);

    auto leftP = std::min({ baseBounds.left, smallBounds.left, bigBounds.left });
    auto topP = std::min({ baseBounds.top, smallBounds.top, bigBounds.top });
    auto rightP = std::max({ right(baseBounds), right(smallBounds), right(bigBounds) });
    auto bottomP = std::max({bottom(baseBounds), bottom(smallBounds), bottom(bigBounds)});

    return getWorldTransform().transformRect(sf::FloatRect(leftP, topP, rightP - leftP, bottomP - topP));
}

bool Tractor::isMarkedForRemoval() const
{
    return isDestroyed() && (!mShowExplosion || mExplosion.isFinished());
}

void Tractor::doDestroy()
{
    Entity::doDestroy();
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
        [effect, worldPosition](SoundNode& node, sf::Time) {
            node.playSound(effect, worldPosition);
        }
    );

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
        createProjectile(node, type, 0.5f, 0.0f, textures);
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
    sf::Vector2f velocity(projectile->getMaxSpeed(), 0);

    projectile->setPosition(getWorldPosition() + offset);
    projectile->setVelocity(velocity);
    node.attachChild(std::move(projectile));
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
