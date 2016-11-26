#pragma once

#include "ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>

class Tractor;

struct Direction
{
    Direction(float angle, float distance)
        : angle(angle)
        , distance(distance)
    {
    }

    float angle;
    float distance;
};

struct TractorData
{
    int hitpoints;
    float speed;
    Textures::ID texture;
    sf::IntRect textureRect;
    sf::Time fireInterval;
    std::vector<Direction> directions;
    bool hasRollAnimation;
};

struct ProjectileData
{
    int damage;
    float speed;
    Textures::ID texture;
    sf::IntRect textureRect;
};

struct PickupData
{
    std::function<void(Tractor&)> action;
    Textures::ID texture;
    sf::IntRect textureRect;
};

struct ParticleData
{
    sf::Color color;
    sf::Time lifetime;
};

std::vector<TractorData>   initializeTractorData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData>     initializePickupData();
std::vector<ParticleData>   initializeParticleData();
