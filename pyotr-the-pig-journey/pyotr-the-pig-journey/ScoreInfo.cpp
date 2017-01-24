#include "stdafx.h"
#include "ScoreInfo.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
    ScoreInfo::ScoreInfo(const sf::Texture& texture, sf::IntRect const& textureRect)
        : mSprite(texture, textureRect)
    {}

    void ScoreInfo::setScore(unsigned score)
    {
        mScore = score;
        auto rect = mSprite.getTextureRect();
        rect.top = mScore * rect.height;
        mSprite.setTextureRect(rect);
    }

    unsigned ScoreInfo::getScore() const
    {
        return mScore;
    }

    void ScoreInfo::operator ++ ()
    {
        ++mScore;
        auto rect = mSprite.getTextureRect();
        rect.top = mScore * rect.height;
        mSprite.setTextureRect(rect);
    }

    void ScoreInfo::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mSprite, states);
    }
}