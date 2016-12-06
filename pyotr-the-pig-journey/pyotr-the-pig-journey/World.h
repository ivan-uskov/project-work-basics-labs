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

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>

class Platform;

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

    sf::FloatRect getBattlefieldBounds() const;

    void createPickup(sf::Vector2f position, Pickup::Type type);

private:
    void loadTextures();
    void adaptPlayerPosition();
    void adaptPlayerVelocity();
    void handleCollisions();
    void updateSounds();

    void buildScene();
    void destroyEntitiesOutsideView();
    void guideMissiles();

    void initializeTractor();
    void initializePlatforms();

private:
    enum Layer
    {
        Background,
        LowerAir,
        UpperAir,
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

    float mScrollSpeed;
    sf::FloatRect mWorldBounds;

    Platform * mGround = nullptr;
    Tractor * mPlayerTractor = nullptr;
    SpriteNode * mFinishSprite = nullptr;
};
