#include "stdafx.h"
#include "Platform.h"

#include "DataTables.h"
#include "Category.h"
#include "CommandQueue.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
    const auto Table = initializePlatformData();
}

Platform::Platform(const sf::Vector2f & size, const TextureHolder& textures, const Type & type)
    : Entity(1)
    , mSize(size)
    , mType(type)
    , mSprite(textures.get(Table[type].texture), Table[type].textureRect)
{
    centerOrigin(mSprite);
}

unsigned int Platform::getCategory() const
{
    return Category::Platform;
}

sf::FloatRect Platform::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Platform::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    auto position = getPosition();
    states.transform.translate(position);
    target.draw(mSprite, states);
}

