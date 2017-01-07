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

        bool isSelectable() const override;
        void handleEvent(const sf::Event& event) override;

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        unsigned mScore = 0;
        sf::Sprite mSprite;
    };
}
