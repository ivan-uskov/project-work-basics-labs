#include "stdafx.h"
#include "TractorView.h"

#include "ResourceHolder.h"
#include "Utility.h"

namespace
{
    const sf::Vector2f SMALL_WHEEL_POS = { 28, 30 };
    const sf::Vector2f BIG_WHEEL_POS = { -5, 30 };
}

TractorView::TractorView(const TextureHolder& textures)
    : mSprite(textures[Textures::PeterOnTractor], sf::IntRect(0, 0, 72, 56))
    , mBigWheelSprite(textures[Textures::PeterOnTractor], sf::IntRect(73, 22, 26, 26))
    , mSmallWheelSprite(textures[Textures::PeterOnTractor], sf::IntRect(72, 0, 21, 21))
{
    centerOrigin(mSprite);
    centerOrigin(mBigWheelSprite);
    centerOrigin(mSmallWheelSprite);

    mSmallWheelSprite.setPosition(SMALL_WHEEL_POS);
    mBigWheelSprite.setPosition(BIG_WHEEL_POS);

    {
        auto rect = getBoundingRect();

        auto dx = -(rect.left + rect.width / 2.f);
        auto dy = -(rect.top + rect.height / 2.f);

        mSprite.move(dx, dy);
        mBigWheelSprite.move(dx, dy);
        mSmallWheelSprite.move(dx, dy);
    }
}

void TractorView::update(sf::Time dt, const sf::Vector2f & velocity)
{
    auto deltha = velocity.x * dt.asSeconds();
    mBigWheelSprite.rotate(deltha);
    mSmallWheelSprite.rotate(deltha);
}

void TractorView::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mBigWheelSprite, states);
    target.draw(mSmallWheelSprite, states);
}

sf::FloatRect TractorView::getBoundingRect() const
{
    auto right = [](auto & rect) {
        return rect.left + rect.width;
    };

    auto bottom = [](auto & rect) {
        return rect.top + rect.height;
    };

    auto getBounds = [](sf::Sprite const& sprite) {
        auto baseBounds = sprite.getLocalBounds();
        baseBounds.left = sprite.getPosition().x - baseBounds.width / 2.f;
        baseBounds.top = sprite.getPosition().y - baseBounds.height / 2.f;

        return baseBounds;
    };

    auto baseBounds = getBounds(mSprite);
    auto smallBounds = getBounds(mSmallWheelSprite);
    auto bigBounds = getBounds(mBigWheelSprite);

    auto leftP = std::min({ baseBounds.left, smallBounds.left, bigBounds.left });
    auto topP = std::min({ baseBounds.top, smallBounds.top, bigBounds.top });
    auto rightP = std::max({ right(baseBounds), right(smallBounds), right(bigBounds) });
    auto bottomP = std::max({ bottom(baseBounds), bottom(smallBounds), bottom(bigBounds) });

    return sf::FloatRect(leftP, topP, rightP - leftP, bottomP - topP);
}