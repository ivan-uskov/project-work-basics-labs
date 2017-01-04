#pragma once

#include "Entity.h"
#include "Command.h"
#include "CommandQueue.h"
#include "ResourceIdentifiers.h"
#include "Projectile.h"
#include "TextNode.h"
#include "Animation.h"

#include <SFML/Graphics/Sprite.hpp>

class Tractor : public Entity
{
public:
    enum Type
    {
        Eagle,
        Raptor,
        Avenger,
        TypeCount
    };

public:
    Tractor(Type type, const TextureHolder& textures, const FontHolder& fonts);

    virtual unsigned int getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;
    virtual void remove();
    virtual bool isMarkedForRemoval() const;
    bool isAllied() const;
    float getMaxSpeed() const;
    void disablePickups();

    void increaseFireRate();
    void increaseSpread();
    void collectMissiles(unsigned int count);

    void fire();
    void launchMissile();
    void playLocalSound(CommandQueue& commands, SoundEffect::ID effect);
    int  getIdentifier();
    void setIdentifier(int identifier);
    int  getMissileAmmo() const;
    void setMissileAmmo(int ammo);

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

    void createBullets(SceneNode& node, const TextureHolder& textures) const;
    void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

    void updateTexts();
    void updateWheelsRotation(sf::Time dt);
    void updateRollAnimation();

private:
    Type mType;
    sf::Sprite mSprite;
    sf::Sprite mBigWheelSprite;
    sf::Sprite mSmallWheelSprite;
    Animation mExplosion;
    Command mFireCommand;
    Command mMissileCommand;
    sf::Time mFireCountdown = sf::Time::Zero;
    bool mIsFiring = false;
    bool mIsLaunchingMissile = false;
    bool mShowExplosion = true;
    bool mExplosionBegan = false;
    bool mSpawnedPickup = false;
    bool mPickupsEnabled = true;

    int mFireRateLevel = 1;
    int mSpreadLevel = 1;
    int mMissileAmmo = 1;

    Command mDropPickupCommand;
    float mTravelledDistance = 0.f;
    std::size_t mDirectionIndex = 0;
    TextNode * mHealthDisplay = nullptr;
    TextNode * mMissileDisplay = nullptr;

    int mIdentifier = 0;
    float mWheelsRotation = 0;
};
