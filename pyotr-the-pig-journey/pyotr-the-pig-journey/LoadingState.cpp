#include "stdafx.h"
#include "LoadingState.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include "ResourceHolder.h"
#include "Utility.h"

namespace
{
    const float PRELOADER_ROTATION_SPEED = 50.f;
}

using namespace std;

LoadingState::LoadingState(StateStack & stack, Context context)
    : State(stack, context)
    , mPreloader(context.textures->get(Textures::Preloader), sf::IntRect(0, 0, 50, 50))
    , mBackground(context.textures->get(Textures::TitleScreen))
{}

void LoadingState::doInitialize()
{
    mText.setString("loading");
    mText.setFont(getContext().fonts->get(Fonts::Main));
    mText.setCharacterSize(40u);

    centerOrigin(mText);
    centerOrigin(mPreloader);
}

void LoadingState::draw()
{
    auto window = getContext().window;

    auto size = window->getSize();

    mPreloader.setPosition(sf::Vector2f(size.x / 2.f, size.y / 2.f));
    mText.setPosition(sf::Vector2f(size.x / 2.f, size.y / 2.f - 60));

    window->draw(mBackground);
    window->draw(mPreloader);
    window->draw(mText);
}

bool LoadingState::update(sf::Time dt)
{
    mPreloader.rotate(PRELOADER_ROTATION_SPEED * dt.asSeconds());
    return true;
}

bool LoadingState::handleEvent(const sf::Event & event)
{
    return true;
}