#pragma once

#include "IPlayerView.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class TractorView : public IPlayerView
{
public:
    TractorView(const TextureHolder& textures);

    void update(sf::Time dt, const sf::Vector2f & velocity) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::FloatRect getBoundingRect() const override;

private:
    sf::Sprite mSprite;
    sf::Sprite mBigWheelSprite;
    sf::Sprite mSmallWheelSprite;
};
