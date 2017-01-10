#include "stdafx.h"
#include "TitleState.h"

#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>


TitleState::TitleState(StateStack & stack, Context context)
    : State(stack, context)
    , mShowText(true)
    , mTextEffectTime(sf::Time::Zero)
{}

void TitleState::initialize()
{
    mBackgroundSprite.setTexture(getContext().textures->get(Textures::TitleScreen));
    mText.setFont(getContext().fonts->get(Fonts::Main));
    mText.setString("Press any key");
    mText.setFillColor(sf::Color::Black);

    centerOrigin(mText);
}

void TitleState::draw()
{
    sf::RenderWindow & window = *(getContext().window);
    window.draw(mBackgroundSprite);

    if (mShowText)
    {
        mText.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y * 2.1f / 3));
        window.draw(mText);
    }
}

bool TitleState::update(sf::Time dt)
{
    mTextEffectTime += dt;

    if (mTextEffectTime >= sf::seconds(0.5f))
    {
        mShowText = !mShowText;
        mTextEffectTime = sf::Time::Zero;
    }

    return true;
}

bool TitleState::handleEvent(const sf::Event & event)
{
    // If any key is pressed, trigger the next screen
    if (event.type == sf::Event::KeyReleased)
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }

    return true;
}