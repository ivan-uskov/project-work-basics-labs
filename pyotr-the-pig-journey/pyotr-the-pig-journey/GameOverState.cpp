#include "stdafx.h"
#include "GameOverState.h"

#include "GameState.h"

#include "Button.h"
#include "Utility.h"
#include "Player.h"
#include "ResourceHolder.h"
#include "StateStack.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

GameOverState::GameOverState(States::ID stateId, StateStack & stack, Context context, const std::string & text)
    : State(stateId, stack, context)
    , mElapsedTime(sf::Time::Zero)
    , mNeedIncludeNextButton(stateId == States::MissionSuccess)
{
    mGameOverText.setString(text);
}

void GameOverState::doInitialize()
{
    auto context = getContext();
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f windowSize(context.window->getSize());

    mGameOverText.setFont(font);
    mGameOverText.setCharacterSize(70);
    centerOrigin(mGameOverText);
    mGameOverText.setPosition(0.5f * windowSize.x, 0.3f * windowSize.y);

    mScoreInfo = std::make_unique<GUI::ScoreInfo>(context.textures->get(Textures::ScoreInfo), sf::IntRect(0, 0, 180, 50));
    mScoreInfo->setPosition(0.5f * windowSize.x - 180.f / 2, windowSize.y * 0.5f);

    float margin = 50.f;
    float buttonX = 150.f + (getId() == States::GameOver ? (GUI::Button::BUTTON_WIDTH + margin) / 2 : 0);
    float buttonY = 0.7f * windowSize.y;

    auto menuButton = std::make_shared<GUI::Button>(context);
    menuButton->setPosition(buttonX, buttonY);
    menuButton->setText("Menu");
    menuButton->setCallback([this] {
        requestStateClear();
        requestStackPush(States::Menu);
    });

    buttonX += GUI::Button::BUTTON_WIDTH + margin;

    auto retryButton = std::make_shared<GUI::Button>(context);
    retryButton->setPosition(buttonX, buttonY);
    retryButton->setText("Retry");
    retryButton->setCallback([this] {
        getState<GameState>(States::Game).prepareReplay();
        requestStackPop();
    });

    mGUIContainer.pack(menuButton);
    mGUIContainer.pack(retryButton);

    if (mNeedIncludeNextButton)
    {
        buttonX += GUI::Button::BUTTON_WIDTH + margin;

        auto nextButton = std::make_shared<GUI::Button>(context);
        nextButton->setPosition(buttonX, buttonY);
        nextButton->setText("Next");
        nextButton->setCallback([this] {
            getState<GameState>(States::Game).prepareNextLevel();
            requestStackPop();
        });

        mGUIContainer.pack(nextButton);
    }
}

void GameOverState::setScore(unsigned score)
{
    mScoreInfo->setScore(score);
}

void GameOverState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    // Create dark, semitransparent background
    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    if (mNeedIncludeNextButton)
    {
        if (mNeedShowNextButton)
        {
            //TODO: hide next button if need
        }
    }


    window.draw(backgroundShape);
    window.draw(mGameOverText);
    window.draw(mGUIContainer);
    window.draw(*mScoreInfo);
}

bool GameOverState::update(sf::Time dt)
{
    return false;
}

bool GameOverState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}

void GameOverState::showNextButtonIf(bool needShowNextButton)
{
    mNeedShowNextButton = needShowNextButton;
}
