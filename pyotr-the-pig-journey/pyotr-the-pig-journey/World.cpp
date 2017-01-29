#include "stdafx.h"
#include "World.h"

#include "TextNode.h"
#include "SoundNode.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);

World::World(sf::RenderTarget & outputTarget, FontHolder & fonts, SoundPlayer & sounds)
    : mTarget(outputTarget)
    , mWorldView(outputTarget.getDefaultView())
    , mFonts(fonts)
    , mSounds(sounds)
{
    initializeLayers();
    initializeTractor();
}

void World::update(sf::Time dt)
{
    while (!mCommandQueue.isEmpty())
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }

    handleCollisions();
    mSceneGraph.removeWrecks();
    mSceneGraph.update(dt, mCommandQueue);

    adaptPlayerPosition();
    updateSounds();
    mWorldView.setCenter(mPlayerTractor->getPosition());
}

void World::draw()
{
    mTarget.setView(mWorldView);
    mTarget.draw(mSceneGraph);
}

CommandQueue & World::getCommandQueue()
{
    return mCommandQueue;
}

bool World::hasAlivePlayer() const
{
    return mPlayerTractor != nullptr;
}

bool World::hasPlayerReachedFinish() const
{
    return mPlayerReachedFinish;
}

void World::initializeLayers()
{
    auto levelLayer = make_unique<SceneNode>();
    mSceneLayers[Layer::Level] = levelLayer.get();
    mSceneGraph.attachChild(move(levelLayer));

    auto playerLayer = make_unique<SceneNode>();
    mSceneLayers[Layer::Player] = playerLayer.get();
    mSceneGraph.attachChild(move(playerLayer));
}

void World::initializeTractor()
{
    mTextures.load(Textures::PeterOnTractor, "Media/PeterViews/PeterOnTractor.png");
    mTextures.load(Textures::Explosion, "Media/Textures/Explosion.png");

    auto player = std::make_unique<PlayerNode>(mTextures, mFonts);
    player->setPosition(mWorldView.getCenter());

    mPlayerTractor = player.get();
    mSceneLayers[Layer::Player]->attachChild(std::move(player));
    mSceneLayers[Layer::Player]->emplaceChild<SoundNode>(mSounds);
}

void World::installLevel(const LevelPtr & level, const LevelTexturesPtr & levelTextures)
{
    mPlayerReachedFinish = false;
    mPlayerTractor->reset();
    mPlayerTractor->setPosition(level->startPos);
    mWorldView.setCenter(level->startPos);

    mSceneLayers[Layer::Level]->removeChildren();
    level->iterateElements([&](const LevelElementInfo & info) {
        auto node = make_unique<SpriteNode>((*levelTextures)[info.key], info.rect, info.type);
        node->setPosition(info.pos);
        mSceneLayers[Layer::Level]->attachChild(move(node));
    });
}

void World::setCollectStarHandler(std::function<void()> && handler)
{
    mCollectStarHandler = move(handler);
}

void World::adaptPlayerPosition()
{
    if (mPlayerTractor)
    {
        sf::FloatRect viewBounds = getViewBounds();
        const float borderDistance = 40.f;

        auto rect = mPlayerTractor->getBoundingRect();

        sf::Vector2f position = mPlayerTractor->getPosition();
        position.x = std::max(position.x, viewBounds.left + borderDistance + rect.width / 2);
        position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance - rect.width / 2);
        position.y = std::max(position.y, viewBounds.top + borderDistance + rect.height / 2);
        position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance - rect.height / 2);

        mPlayerTractor->setPosition(position);
    }
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

    for (SceneNode::Pair pair : collisionPairs)
    {
        if (matchesCategories(pair, Category::Tractor, Category::Finish))
        {
            mPlayerReachedFinish = true;
        }
        else if (matchesCategories(pair, Category::Entity, Category::Platform))
        {
            auto& entity = static_cast<Entity&>(*pair.first);
            entity.addCollision(pair.second);
        }
        else if (matchesCategories(pair, Category::Tractor, Category::Star))
        {
            pair.second->destroy();
            auto & player = static_cast<PlayerNode&>(*pair.first);
            player.playLocalSound(mCommandQueue, SoundEffect::CollectStar);

            if (mCollectStarHandler)
            {
                mCollectStarHandler();
            }
        }
    }
}

void World::updateSounds()
{
    sf::Vector2f listenerPosition;

    if (mPlayerTractor)
    {
        listenerPosition = mPlayerTractor->getWorldPosition();
    }
    else
    {
        listenerPosition = mWorldView.getCenter();
    }

    mSounds.setListenerPosition(listenerPosition);
    mSounds.removeStoppedSounds();
}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();

    // Make sure first pair entry has category type1 and second has type2
    if ((type1 & category1) && (type2 & category2))
    {
        return true;
    }
    else if ((type1 & category2) && (type2 & category1))
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}