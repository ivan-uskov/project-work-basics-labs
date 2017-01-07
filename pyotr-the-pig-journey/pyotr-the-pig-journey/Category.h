#pragma once

namespace Category
{
    enum Type
    {
        None = 0,
        SceneAirLayer = 1 << 0,
        Tractor = 1 << 1,
        Platform = 1 << 2,
        Finish = 1 << 3,
        Pickup = 1 << 4,
        AlliedProjectile = 1 << 5,
        EnemyProjectile = 1 << 6,
        ParticleSystem = 1 << 7,
        SoundEffect = 1 << 8,

        CanCollide = Tractor | Finish | Platform,
        Projectile = AlliedProjectile | EnemyProjectile,
        Entity = Tractor | Pickup | Projectile
    };
}