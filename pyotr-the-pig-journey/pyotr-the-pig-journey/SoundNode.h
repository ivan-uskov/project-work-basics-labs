#pragma once

#include "SceneNode.h"
#include "ResourceIdentifiers.h"

class SoundPlayer;

class SoundNode : public SceneNode
{
public:
    explicit SoundNode(SoundPlayer& player);
    void playSound(SoundEffect::ID sound, sf::Vector2f position);

private:
    SoundPlayer & mSounds;
};
