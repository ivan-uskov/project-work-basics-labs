#pragma once

#include "Entity.h"
#include "Command.h"
#include "CommandQueue.h"
#include "ResourceIdentifiers.h"
#include "TextNode.h"
#include "Animation.h"

#include "TractorView.h"

#include <SFML/Graphics/Sprite.hpp>

class PlayerNode : public Entity
{
public:
    PlayerNode(const TextureHolder & textures, const FontHolder & fonts);

    void doDestroy() override;
    bool isMarkedForRemoval() const override;

    void playLocalSound(CommandQueue& commands, SoundEffect::ID effect);

    sf::FloatRect getBoundingRect() const override;

protected:
    void updateCurrent(sf::Time dt, CommandQueue& commands) override;
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    Animation mExplosion;

    bool mShowExplosion = true;
    bool mExplosionBegan = false;

    TractorView mView;
};
