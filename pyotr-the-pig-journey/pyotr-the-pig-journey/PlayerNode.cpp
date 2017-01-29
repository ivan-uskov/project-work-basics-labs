#include "stdafx.h"
#include "PlayerNode.h"

#include "Utility.h"
#include "CommandQueue.h"
#include "SoundNode.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

using namespace std::placeholders;

PlayerNode::PlayerNode(const TextureHolder& textures, const FontHolder& fonts)
    : Entity(100, Category::Tractor)
    , mExplosion(textures[Textures::Explosion])
    , mView(textures)
{
    mExplosion.setFrameSize(sf::Vector2i(256, 256));
    mExplosion.setNumFrames(16);
    mExplosion.setDuration(sf::seconds(1));
}

void PlayerNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (SceneNode::isMarkedForRemoval() && mShowExplosion)
    {
        target.draw(mExplosion, states);
    }
    else
    {
        mView.draw(target, states);
    }
}

void PlayerNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
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
    mView.update(dt, getVelocity());
}

sf::FloatRect PlayerNode::getBoundingRect() const
{
    return getWorldTransform().transformRect(mView.getBoundingRect());
}

bool PlayerNode::isMarkedForRemoval() const
{
    return SceneNode::isMarkedForRemoval() && (!mShowExplosion || mExplosion.isFinished());
}

void PlayerNode::doDestroy()
{
    Entity::doDestroy();
    mShowExplosion = false;
}

void PlayerNode::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
    auto worldPosition = getWorldPosition();
    commands.push(Command(Category::SoundEffect, derivedAction<SoundNode>(
        [effect, &worldPosition](SoundNode& node, sf::Time) {
            node.playSound(effect, worldPosition);
        }
    )));
}