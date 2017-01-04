#pragma once

#include "State.h"
#include "World.h"
#include "Player.h"
#include "LevelInfo.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State
{
public:
    GameState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

    void initialize();

private:
    World mWorld;
    Player mPlayer;
    Levels mLevels;
    LevelTexturesPtr mLevelTextures;
};