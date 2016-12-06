#include "stdafx.h"
#include "World.h"

#include "Platform.h"
#include "Projectile.h"
#include "Pickup.h"
#include "TextNode.h"
#include "ParticleNode.h"
#include "SoundNode.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

World::World(sf::RenderTarget & outputTarget, FontHolder & fonts, SoundPlayer & sounds)
    : mTarget(outputTarget)
    , mWorldView(outputTarget.getDefaultView())
    , mFonts(fonts)
    , mSounds(sounds)
    , mWorldBounds(0.f, 0.f, 5000.f, mWorldView.getSize().y)
    , mScrollSpeed(50.f)
{
    loadTextures();
    buildScene();

    mWorldView.setCenter(mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 2.f);
    initializePlatforms();
    initializeTractor();
}

void World::update(sf::Time dt)
{
    //mWorldView.move(mScrollSpeed * dt.asSeconds(), 0.f);

    mPlayerTractor->setVelocity(0.f, 0.f);

    destroyEntitiesOutsideView();
    guideMissiles();

    while (!mCommandQueue.isEmpty())
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }

    adaptPlayerVelocity();
    handleCollisions();
    mSceneGraph.removeWrecks();
    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();
    updateSounds();
}

void World::draw()
{
    mTarget.setView(mWorldView);
    mTarget.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::initializeTractor()
{
    std::unique_ptr<Tractor> player(new Tractor(Tractor::Eagle, mTextures, mFonts));
    player->setPosition(mWorldView.getCenter());
    player->setIdentifier(1);

    mPlayerTractor = player.get();
    mSceneLayers[UpperAir]->attachChild(std::move(player));
}

void World::initializePlatforms()
{
    auto grass = mTextures.get(Textures::Grass);
    grass.setRepeated(true);

    auto viewCenter = mWorldView.getCenter();

    auto width = mWorldBounds.width;
    auto height = mWorldBounds.height / 4;
    std::unique_ptr<Platform> ground(new Platform(sf::Vector2f(width, height), mTextures));
    ground->setPosition(width / 2, mWorldBounds.height - height + height / 2);

    mSceneLayers[LowerAir]->attachChild(std::move(ground));
}

void World::createPickup(sf::Vector2f position, Pickup::Type type)
{
    std::unique_ptr<Pickup> pickup(new Pickup(type, mTextures));
    pickup->setPosition(position);
    pickup->setVelocity(0.f, 1.f);
    mSceneLayers[UpperAir]->attachChild(std::move(pickup));
}

bool World::hasAlivePlayer() const
{
    return mPlayerTractor != nullptr;
}

bool World::hasPlayerReachedEnd() const
{
    if (mPlayerTractor)
    {
        return !mWorldBounds.contains(mPlayerTractor->getPosition());
    }
    else
    {
        return false;
    }
}

void World::loadTextures()
{
    mTextures.load(Textures::Entities, "Media/Textures/Entities.png");
    mTextures.load(Textures::Clouds, "Media/Textures/Clouds.png");
    mTextures.load(Textures::Grass, "Media/Textures/Grass.png");
    mTextures.load(Textures::Explosion, "Media/Textures/Explosion.png");
    mTextures.load(Textures::Particle, "Media/Textures/Particle.png");
    mTextures.load(Textures::FinishLine, "Media/Textures/FinishLine.png");
}

void World::adaptPlayerPosition()
{
    if (mPlayerTractor)
    {
        // Keep player's position inside the screen bounds, at least borderDistance units from the border
        sf::FloatRect viewBounds = getViewBounds();
        const float borderDistance = 40.f;

        sf::Vector2f position = mPlayerTractor->getPosition();
        position.x = std::max(position.x, viewBounds.left + borderDistance);
        position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
        position.y = std::max(position.y, viewBounds.top + borderDistance);
        position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
        mPlayerTractor->setPosition(position);
    }
}

void World::adaptPlayerVelocity()
{
    if (mPlayerTractor)
    {
        sf::Vector2f velocity = mPlayerTractor->getVelocity();

        // If moving diagonally, reduce velocity (to have always same velocity)
        if (velocity.x != 0.f && velocity.y != 0.f)
        {
            mPlayerTractor->setVelocity(velocity / std::sqrt(2.f));
        }

        // Add scrolling velocity
        //mPlayerTractor->accelerate(mScrollSpeed, 0.f);
    }
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

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

    for (SceneNode::Pair pair : collisionPairs)
    {
        if (matchesCategories(pair, Category::PlayerTractor, Category::EnemyTractor))
        {
            auto& player = static_cast<Tractor&>(*pair.first);
            auto& enemy = static_cast<Tractor&>(*pair.second);

            // Collision: Player damage = enemy's remaining HP
            player.damage(enemy.getHitpoints());
            enemy.destroy();
        }
        else if (matchesCategories(pair, Category::PlayerTractor, Category::Pickup))
        {
            auto& player = static_cast<Tractor&>(*pair.first);
            auto& pickup = static_cast<Pickup&>(*pair.second);

            pickup.apply(player);
            pickup.destroy();
            player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
        }
        else if (matchesCategories(pair, Category::EnemyTractor, Category::AlliedProjectile)
            || matchesCategories(pair, Category::PlayerTractor, Category::EnemyProjectile))
        {
            auto& tractor = static_cast<Tractor&>(*pair.first);
            auto& projectile = static_cast<Projectile&>(*pair.second);

            // Apply projectile damage to tractor, destroy projectile
            tractor.damage(projectile.getDamage());
            projectile.destroy();
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

void World::buildScene()
{
    for (std::size_t i = 0; i < LayerCount; ++i)
    {
        Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

        auto layer = std::make_unique<SceneNode>(category);
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    {
        sf::Texture & cloudsTexture = mTextures.get(Textures::Clouds);
        cloudsTexture.setRepeated(true);

        float viewWidth = mWorldView.getSize().x;
        sf::IntRect textureRect(mWorldBounds);
        textureRect.width += static_cast<int>(viewWidth);

        // Add the background sprite to the scene
        auto cloudsSprite = std::make_unique<SpriteNode>(cloudsTexture, textureRect);
        cloudsSprite->setPosition(0.f, mWorldBounds.top);
        mSceneLayers[Background]->attachChild(std::move(cloudsSprite));
        // Prepare the tiled background
    }

    {
        // Add the finish line to the scene
        sf::Texture& finishTexture = mTextures.get(Textures::FinishLine);
        auto finishSprite = std::make_unique<SpriteNode>(finishTexture);
        finishSprite->setPosition(0.f, -76.f);
        mFinishSprite = finishSprite.get();
        mSceneLayers[Background]->attachChild(std::move(finishSprite));
    }

    // Add particle node to the scene
    auto smokeNode = std::make_unique<ParticleNode>(Particle::Smoke, mTextures);
    mSceneLayers[LowerAir]->attachChild(std::move(smokeNode));

    // Add propellant particle node to the scene
    auto propellantNode = std::make_unique<ParticleNode>(Particle::Propellant, mTextures);
    mSceneLayers[LowerAir]->attachChild(std::move(propellantNode));

    // Add sound effect node
    auto soundNode = std::make_unique<SoundNode>(mSounds);
    mSceneGraph.attachChild(std::move(soundNode));
}

void World::destroyEntitiesOutsideView()
{
    mCommandQueue.emplace(
        Category::Projectile | Category::EnemyTractor,
        derivedAction<Entity>([this](Entity& e, sf::Time) {
            if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
            {
                e.remove();
            }
        })
    );
}

void World::guideMissiles()
{
    mCommandQueue.emplace(
        Category::AlliedProjectile,
        derivedAction<Projectile>([this](Projectile& missile, sf::Time) {
            if (!missile.isGuided())
            {
                return;
            }
        })
    );
}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
    return getViewBounds();
}
