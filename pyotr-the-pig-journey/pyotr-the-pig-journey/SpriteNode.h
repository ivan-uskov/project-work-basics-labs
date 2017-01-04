#pragma once

#include "SceneNode.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode
{
public:
    SpriteNode(const sf::Texture & texture, const sf::IntRect& textureRect, const NodeType & nodeType);

    sf::FloatRect getBoundingRect() const override;

private:
    virtual void drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const;

private:
    sf::Sprite mSprite;
    NodeType mNodeType;
};
