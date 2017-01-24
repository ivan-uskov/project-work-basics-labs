#pragma once

#include "ResourceIdentifiers.h"
#include "Component.h"

#include <SFML/Graphics/Sprite.hpp>

namespace GUI
{
    class ScoreInfo : public Component
    {
    public:
        ScoreInfo(const sf::Texture& texture, sf::IntRect const& textureRect);

        void operator ++ ();

        void setScore(unsigned score);
        unsigned getScore() const;

    protected:
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        unsigned mScore = 0;
        sf::Sprite mSprite;
    };
}
