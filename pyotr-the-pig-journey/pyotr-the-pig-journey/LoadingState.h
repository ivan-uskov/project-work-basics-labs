#pragma once

#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class LoadingState : public State
{
public:
    LoadingState(StateStack & stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

protected:
    void doInitialize() override;

private:
    sf::Sprite mBackground;
    sf::Sprite mPreloader;
    sf::Text mText;
};

