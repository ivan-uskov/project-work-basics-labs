#include "stdafx.h"
#include "SoundNode.h"

#include "SoundPlayer.h"

SoundNode::SoundNode(SoundPlayer& player)
    : SceneNode(Category::SoundEffect)
    , mSounds(player)
{}

void SoundNode::playSound(SoundEffect::ID sound, sf::Vector2f position)
{
    mSounds.play(sound, position);
}
