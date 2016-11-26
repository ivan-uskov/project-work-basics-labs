#include "stdafx.h"
#include "MenuState.h"

#include "Button.h"
#include "Utility.h"
#include "MusicPlayer.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context)
    , mGUIContainer()
{
    sf::Texture& texture = context.textures->get(Textures::TitleScreen);
    mBackgroundSprite.setTexture(texture);

    float buttonX = 100.f;
    float buttonY = 400.f;
    float margin = 5.f;

    auto playButton = std::make_shared<GUI::Button>(context);
    playButton->setPosition(buttonX, buttonY);
    playButton->setText("Play");
    playButton->setCallback([this]()
    {
        requestStackPop();
        requestStackPush(States::Game);
    });

    buttonY += GUI::Button::BUTTON_HEIGHT + margin;

    auto settingsButton = std::make_shared<GUI::Button>(context);
    settingsButton->setPosition(buttonX, buttonY);
    settingsButton->setText("Settings");
    settingsButton->setCallback([this]()
    {
        requestStackPush(States::Settings);
    });

    buttonY += GUI::Button::BUTTON_HEIGHT + margin;

    auto exitButton = std::make_shared<GUI::Button>(context);
    exitButton->setPosition(buttonX, buttonY);
    exitButton->setText("Exit");
    exitButton->setCallback([this]()
    {
        requestStackPop();
    });

    mGUIContainer.pack(playButton);
    mGUIContainer.pack(settingsButton);
    mGUIContainer.pack(exitButton);

    context.music->play(Music::MenuTheme);
}

void MenuState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
    return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}
