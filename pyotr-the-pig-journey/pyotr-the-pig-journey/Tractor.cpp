#include "stdafx.h"
#include "Tractor.h"

#include "Utility.h"
#include "CommandQueue.h"
#include "SoundNode.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

using namespace std::placeholders;

namespace
{
    const sf::Vector2f SMALL_WHEEL_POS = { 55, 62 };
    const sf::Vector2f BIG_WHEEL_POS = { -10, 50 };
}

Tractor::Tractor(const TextureHolder& textures, const FontHolder& fonts)
    : Entity(100, Category::Tractor)
    , mSprite(textures[Textures::Entities], sf::IntRect(0, 105, 145, 113))
    , mBigWheelSprite(textures[Textures::Entities], sf::IntRect(152, 109, 66, 67))
    , mSmallWheelSprite(textures[Textures::Entities], sf::IntRect(190, 68, 39, 39))
    , mExplosion(textures[Textures::Explosion])
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

    auto healthDisplay = std::make_unique<TextNode>(fonts, "");
    healthDisplay->setPosition(0.f, -90.f);
    mHealthDisplay = healthDisplay.get();
    attachChild(std::move(healthDisplay));

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
    }
}

void Tractor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (SceneNode::isMarkedForRemoval() && mShowExplosion)
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

void Tractor::updateWheelsRotation(sf::Time dt)
{
    mBigWheelSprite.rotate(getVelocity().x * dt.asSeconds());
    mSmallWheelSprite.rotate(getVelocity().x * dt.asSeconds());
}

void Tractor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    updateTexts();
    updateWheelsRotation(dt);

    if (SceneNode::isMarkedForRemoval())
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
    return SceneNode::isMarkedForRemoval() && (!mShowExplosion || mExplosion.isFinished());
}

void Tractor::doDestroy()
{
    Entity::doDestroy();
    mShowExplosion = false;
}

void Tractor::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
    auto worldPosition = getWorldPosition();
    commands.push(Command(Category::SoundEffect, derivedAction<SoundNode>(
        [effect, &worldPosition](SoundNode& node, sf::Time) {
            node.playSound(effect, worldPosition);
        }
    )));
}

int Tractor::getIdentifier()
{
    return mIdentifier;
}

void Tractor::setIdentifier(int identifier)
{
    mIdentifier = identifier;
}

void Tractor::updateTexts()
{
    mHealthDisplay->setString(SceneNode::isMarkedForRemoval() ? "" : toString(getHitpoints()) + " HP");
    mHealthDisplay->setRotation(-getRotation());
}
