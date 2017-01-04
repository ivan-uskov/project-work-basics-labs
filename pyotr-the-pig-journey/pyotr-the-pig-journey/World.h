#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Tractor.h"
#include "CommandQueue.h"
#include "Command.h"
#include "Pickup.h"
#include "SoundPlayer.h"

#include "LevelInfo.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>

// Forward declaration
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
    bool hasPlayerReachedEnd() const;

    void installLevel(const LevelPtr & level, const LevelTexturesPtr & levelTextures);

private:
    void adaptPlayerPosition();
    void handleCollisions();
    void updateSounds();

    void guideMissiles();

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

    SceneNode mSceneGraph;
    std::array<SceneNode*, LayerCount> mSceneLayers;
    CommandQueue mCommandQueue;

    Tractor * mPlayerTractor = nullptr;
};
