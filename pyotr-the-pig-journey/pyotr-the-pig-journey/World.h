#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "PlayerNode.h"
#include "CommandQueue.h"
#include "Command.h"
#include "SoundPlayer.h"
#include "LevelInfo.h"

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>

namespace sf
{
    class RenderTarget;
}

class World : private sf::NonCopyable
{
public:
    World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);

    void update(sf::Time dt);
    void draw();

    sf::FloatRect getViewBounds() const;
    CommandQueue & getCommandQueue();

    bool hasAlivePlayer() const;
    bool hasPlayerReachedFinish() const;

    void installLevel(const LevelPtr & level, const LevelTexturesPtr & levelTextures);

    void setCollectStarHandler(std::function<void()> && handler);

private:
    void adaptPlayerPosition();
    void handleCollisions();
    void updateSounds();

    void initializeTractor();
    void initializeLayers();

private:
    enum Layer
    {
        Level,
        Player,
        LayerCount
    };

private:
    sf::RenderTarget & mTarget;
    sf::View mWorldView;
    TextureHolder mTextures;
    FontHolder & mFonts;
    SoundPlayer & mSounds;

    std::function<void()> mCollectStarHandler;

    SceneNode mSceneGraph;
    std::array<SceneNode*, LayerCount> mSceneLayers;
    CommandQueue mCommandQueue;

    PlayerNode * mPlayerTractor = nullptr;
    bool mPlayerReachedFinish = false;
};
