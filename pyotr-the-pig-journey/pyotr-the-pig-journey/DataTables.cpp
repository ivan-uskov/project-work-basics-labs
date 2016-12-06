#include "stdafx.h"
#include "DataTables.h"

#include "Tractor.h"
#include "Projectile.h"
#include "Pickup.h"
#include "Platform.h"
#include "Particle.h"

using namespace std::placeholders;

std::vector<TractorData> initializeTractorData()
{
    std::vector<TractorData> data(Tractor::TypeCount);

    data[Tractor::Eagle].hitpoints = 100;
    data[Tractor::Eagle].speed = 200.f;
    data[Tractor::Eagle].fireInterval = sf::seconds(1);
    data[Tractor::Eagle].texture = Textures::Entities;
    data[Tractor::Eagle].textureRect = sf::IntRect(0, 104, 145, 113);
    data[Tractor::Eagle].bigWheelTextureRect = sf::IntRect(152, 109, 66, 67);
    data[Tractor::Eagle].smallWheelTextureRect = sf::IntRect(190, 68, 39, 39);

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);

    data[Projectile::AlliedBullet].damage = 10;
    data[Projectile::AlliedBullet].speed = 300.f;
    data[Projectile::AlliedBullet].texture = Textures::Entities;
    data[Projectile::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

    data[Projectile::EnemyBullet].damage = 10;
    data[Projectile::EnemyBullet].speed = 300.f;
    data[Projectile::EnemyBullet].texture = Textures::Entities;
    data[Projectile::EnemyBullet].textureRect = sf::IntRect(178, 64, 3, 14);

    data[Projectile::Missile].damage = 200;
    data[Projectile::Missile].speed = 150.f;
    data[Projectile::Missile].texture = Textures::Entities;
    data[Projectile::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

    return data;
}

std::vector<PickupData> initializePickupData()
{
    std::vector<PickupData> data(Pickup::TypeCount);

    data[Pickup::HealthRefill].texture = Textures::Entities;
    data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
    data[Pickup::HealthRefill].action = [](Tractor& a) { a.repair(25); };

    data[Pickup::MissileRefill].texture = Textures::Entities;
    data[Pickup::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
    data[Pickup::MissileRefill].action = std::bind(&Tractor::collectMissiles, _1, 3);

    data[Pickup::FireSpread].texture = Textures::Entities;
    data[Pickup::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
    data[Pickup::FireSpread].action = std::bind(&Tractor::increaseSpread, _1);

    data[Pickup::FireRate].texture = Textures::Entities;
    data[Pickup::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
    data[Pickup::FireRate].action = std::bind(&Tractor::increaseFireRate, _1);

    return data;
}

std::vector<PlatformData> initializePlatformData()
{
    std::vector<PlatformData> data(Platform::TypeCount);

    data[Platform::Ground].texture = Textures::Grass;
    data[Platform::Ground].textureRect = sf::IntRect(0, 0, 288, 72);

    return data;
}

std::vector<ParticleData> initializeParticleData()
{
    std::vector<ParticleData> data(Particle::ParticleCount);

    data[Particle::Propellant].color = sf::Color(255, 255, 50);
    data[Particle::Propellant].lifetime = sf::seconds(0.6f);

    data[Particle::Smoke].color = sf::Color(50, 50, 50);
    data[Particle::Smoke].lifetime = sf::seconds(4.f);

    return data;
}
