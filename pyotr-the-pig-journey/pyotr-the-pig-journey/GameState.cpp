#include "stdafx.h"
#include "GameState.h"

#include "MusicPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack & stack, Context context)
    : State(stack, context)
    , mWorld(*context.window, *context.fonts, *context.sounds)
    , mPlayer(1, context.keys1)
{
    //context.music->play(Music::MissionTheme);
}

void GameState::doInitialize()
{
    mPlayer.setMissionStatus(Player::MissionRunning);
    loadTextures();

    mScoreInfo = std::make_unique<GUI::ScoreInfo>(mTextures.get(Textures::ScoreInfo), sf::IntRect(0, 0, 180, 50));
    mWorld.setCollectStarHandler([&] {
        ++(*mScoreInfo);
    });

    mWorld.installLevel(mLevels[0], mLevelTextures);
}

void GameState::loadTextures()
{
    mLevels = getLevels("Media/scene_list.conf");
    mLevelTextures = std::make_shared<LevelTextureHolder>();
    for (auto const& level : mLevels)
    {
        level->iterateElements([&](auto const& textureInfo) {
            auto key = textureInfo.key;
            mLevelTextures->load(key, "Media/" + key);
            mLevelTextures->get(key).setRepeated(textureInfo.repeated);
        });
    }

    mTextures.load(Textures::ScoreInfo, "Media/Textures/ScoreInfo.png");
}

void GameState::draw()
{
    mWorld.draw();

    auto & window = *getContext().window;
    auto width = window.getSize().x;
    mScoreInfo->setPosition(sf::Vector2f(width - 180.f - 15, 15));
    window.setView(window.getDefaultView());
    window.draw(*mScoreInfo);
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);

    if (!mWorld.hasAlivePlayer())
    {
        mPlayer.setMissionStatus(Player::MissionFailure);
        requestStackPush(States::GameOver);
    }
    else if (mWorld.hasPlayerReachedFinish())
    {
        mPlayer.setMissionStatus(Player::MissionSuccess);
        requestStackPush(States::MissionSuccess);
    }

    auto & commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);

    return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
    CommandQueue & commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestStackPush(States::Pause);
    }

    return true;
}