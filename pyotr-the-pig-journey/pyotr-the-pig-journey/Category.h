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
        Star = 1 << 9,
        Destroyer = 1 << 10,

        CenterOriginSprite = Star,
        CanCollide = Tractor | Finish | Platform | Star | Destroyer,
        Projectile = AlliedProjectile | EnemyProjectile,
        Entity = Tractor | Pickup | Projectile
    };
}