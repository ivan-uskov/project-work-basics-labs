#pragma once

#include "State.h"
#include "Container.h"
#include "ScoreInfo.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameOverState : public State
{
public:
    GameOverState(States::ID stateId, StateStack & stack, Context context, const std::string& text);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

    void setScore(unsigned score);
    void showNextButtonIf(bool needShowNextButton);

protected:
    void doInitialize() override;

private:
    enum Button
    {
        Menu = 0,
        Retry = 1,
        Next = 2
    };

    bool mNeedShowNextButton = false;
    bool mNeedIncludeNextButton;
    sf::Text mGameOverText;
    sf::Time mElapsedTime;
    GUI::Container mGUIContainer;
    std::unique_ptr<GUI::ScoreInfo> mScoreInfo;
};