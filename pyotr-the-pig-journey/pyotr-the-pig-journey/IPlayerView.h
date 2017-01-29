#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class IPlayerView
{
public:
    virtual void update(sf::Time dt, const sf::Vector2f & velocity) = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

    virtual sf::FloatRect getBoundingRect() const = 0;

    virtual ~IPlayerView() = default;
};
