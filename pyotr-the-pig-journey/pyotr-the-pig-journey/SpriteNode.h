#pragma once

#include "SceneNode.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode
{
public:
    SpriteNode(const sf::Texture & texture, const sf::IntRect& textureRect, Category::Type category);

    sf::FloatRect getBoundingRect() const override;

protected:
    void drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    sf::Sprite mSprite;
};
