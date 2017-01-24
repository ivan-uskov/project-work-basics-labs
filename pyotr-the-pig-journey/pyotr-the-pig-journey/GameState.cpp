#include "stdafx.h"
#include "GameState.h"
#include "StateStack.h"
#include "GameOverState.h"

#include "MusicPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(States::ID stateId, StateStack & stack, Context context)
    : State(stateId, stack, context)
    , mWorld(context.window, context.fonts, context.sounds)
    , mPlayer(1, &context.keys1)
{
    //context.music->play(Music::MissionTheme);
}

void GameState::doInitialize()
{
    mPlayer.setMissionStatus(Player::MissionRunning);
    loadTextures();

    mScoreInfo = std::make_unique<GUI::ScoreInfo>(getContext().textures[Textures::ScoreInfo], sf::IntRect(0, 0, 180, 50));
    mWorld.setCollectStarHandler([&] {
        ++(*mScoreInfo);
    });
}

void GameState::loadTextures()
{
    mLevels = getLevels("Media/scene_list.conf");
    mLevelTextures = std::make_shared<LevelTextureHolder>();
    for (auto const& level : mLevels)
    {
        level->iterateElements([&](auto const& textureInfo) {
            auto key = textureInfo.key;
            mLevelTextures->load(key, "Media/" + key).setRepeated(textureInfo.repeated);
        });
    }
}

void GameState::prepareFirstLevel()
{
    mCurrentLevel = 0;
    mWorld.installLevel(mLevels[mCurrentLevel], mLevelTextures);
}

void GameState::prepareReplay()
{
    mWorld.installLevel(mLevels[mCurrentLevel], mLevelTextures);
}

void GameState::prepareNextLevel()
{
    mCurrentLevel = std::min(++mCurrentLevel, mLevels.size() - 1);
    mWorld.installLevel(mLevels[mCurrentLevel], mLevelTextures);
}

void GameState::draw()
{
    mWorld.draw();

    auto & window = getContext().window;
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
        auto score = mScoreInfo->getScore();
        if (score == 0)
        {
            mPlayer.setMissionStatus(Player::MissionFailure);
            requestStackPush(States::GameOver);
        }
        else
        {
            mPlayer.setMissionStatus(Player::MissionSuccess);
            auto & gameOverState = getState<GameOverState>(States::MissionSuccess);
            gameOverState.setScore(mScoreInfo->getScore());
            gameOverState.showNextButtonIf(hasNextLevel());
            requestStackPush(States::MissionSuccess);
        }
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

void GameState::onActivate()
{
    mScoreInfo->setScore(0);
}

bool GameState::hasNextLevel() const
{
    return (mCurrentLevel + 1) < mLevels.size();
}