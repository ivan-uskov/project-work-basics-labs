#include "stdafx.h"
#include "ScoreInfo.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
    ScoreInfo::ScoreInfo(const sf::Texture& texture, sf::IntRect const& textureRect)
        : mSprite(texture, textureRect)
    {
    }

    bool ScoreInfo::isSelectable() const
    {
        return false;
    }

    void ScoreInfo::handleEvent(const sf::Event&)
    {
    }

    void ScoreInfo::operator ++ ()
    {
        ++mScore;
        auto rect = mSprite.getTextureRect();
        rect.top = mScore * rect.height;
        mSprite.setTextureRect(rect);
    }

    void ScoreInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(mSprite, states);
    }
}