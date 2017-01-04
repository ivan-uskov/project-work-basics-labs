#include "stdafx.h"
#include "GameState.h"

#include "MusicPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack & stack, Context context)
    : State(stack, context)
    , mWorld(*context.window, *context.fonts, *context.sounds)
    , mPlayer(1, context.keys1)
{
    mPlayer.setMissionStatus(Player::MissionRunning);

    context.music->play(Music::MissionTheme);

    //TODO: run async
    initialize();
}

void GameState::initialize()
{
    mLevels = getLevels("Media/scene_list.conf");
    mLevelTextures = std::make_shared<LevelTextureHolder>();
    auto & currentLevel = mLevels[0];
    currentLevel->iterateElements([&](auto const& textureInfo) {
        auto key = textureInfo.key;
        mLevelTextures->load(key, "Media/" + key);
        mLevelTextures->get(key).setRepeated(textureInfo.repeated);
    });
    mWorld.installLevel(currentLevel, mLevelTextures);
}

void GameState::draw()
{
    mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);

    if (!mWorld.hasAlivePlayer())
    {
        mPlayer.setMissionStatus(Player::MissionFailure);
        requestStackPush(States::GameOver);
    }
    else if (mWorld.hasPlayerReachedEnd())
    {
        mPlayer.setMissionStatus(Player::MissionSuccess);
        requestStackPush(States::MissionSuccess);
    }

    CommandQueue & commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);

    return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
    // Game input handling
    CommandQueue & commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    // Escape pressed, trigger the pause screen
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestStackPush(States::Pause);
    }

    return true;
}