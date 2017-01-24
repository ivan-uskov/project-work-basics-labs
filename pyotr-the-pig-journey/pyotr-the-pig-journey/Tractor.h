#pragma once

#include "Entity.h"
#include "Command.h"
#include "CommandQueue.h"
#include "ResourceIdentifiers.h"
#include "TextNode.h"
#include "Animation.h"

#include <SFML/Graphics/Sprite.hpp>

class Tractor : public Entity
{
public:
    Tractor(const TextureHolder& textures, const FontHolder& fonts);

    virtual sf::FloatRect getBoundingRect() const;
    void doDestroy() override;
    bool isMarkedForRemoval() const override;

    void playLocalSound(CommandQueue& commands, SoundEffect::ID effect);
    int  getIdentifier();
    void setIdentifier(int identifier);

protected:
    void updateCurrent(sf::Time dt, CommandQueue& commands) override;
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void updateTexts();
    void updateWheelsRotation(sf::Time dt);

private:
    sf::Sprite mSprite;
    sf::Sprite mBigWheelSprite;
    sf::Sprite mSmallWheelSprite;
    Animation mExplosion;

    bool mShowExplosion = true;
    bool mExplosionBegan = false;

    TextNode * mHealthDisplay = nullptr;

    int mIdentifier = 0;
    float mWheelsRotation = 0;
};
