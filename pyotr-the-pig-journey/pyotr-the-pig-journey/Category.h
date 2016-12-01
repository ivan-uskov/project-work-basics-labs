#pragma once

// Entity/scene node category, used to dispatch commands
namespace Category
{
    enum Type
    {
        None = 0,
        SceneAirLayer = 1 << 0,
        PlayerTractor = 1 << 1,
        AlliedTractor = 1 << 2,
        EnemyTractor = 1 << 3,
        Platform = 1 << 4,
        Pickup = 1 << 5,
        AlliedProjectile = 1 << 6,
        EnemyProjectile = 1 << 7,
        ParticleSystem = 1 << 8,
        SoundEffect = 1 << 9,

        Tractor = PlayerTractor | AlliedTractor | EnemyTractor,
        Projectile = AlliedProjectile | EnemyProjectile,
    };
}