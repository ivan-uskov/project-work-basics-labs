#pragma once

#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState : public State
{
public:
    PauseState(States::ID stateId, StateStack& stack, Context context, bool letUpdatesThrough = false);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

    void onActivate() override;
    void onDeactivate() override;

protected:
    void doInitialize() override;

private:
    sf::Sprite mBackgroundSprite;
    sf::Text mPausedText;
    GUI::Container mGUIContainer;
    bool mLetUpdatesThrough;
};