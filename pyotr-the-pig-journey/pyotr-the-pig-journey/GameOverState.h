#pragma once

#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameOverState : public State
{
public:
    GameOverState(StateStack & stack, Context context, const std::string& text);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

    void initialize() override;

private:
    sf::Text mGameOverText;
    sf::Time mElapsedTime;
};