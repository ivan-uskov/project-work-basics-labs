#pragma once

#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class Platform : public Entity
{
public:
    enum Type
    {
        Ground,
        TypeCount
    };

    Platform(const sf::Vector2f & size, const TextureHolder& textures, const Type & type = Ground);

    virtual unsigned int getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;

protected:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Vector2f mSize;
    Type mType;
    sf::Sprite mSprite;
};

