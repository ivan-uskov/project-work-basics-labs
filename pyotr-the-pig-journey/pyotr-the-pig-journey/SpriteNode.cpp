#include "stdafx.h"
#include "SpriteNode.h"

#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect, const NodeType & nodeType)
    : mSprite(texture, textureRect)
    , mNodeType(nodeType)
{
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

sf::FloatRect SpriteNode::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}