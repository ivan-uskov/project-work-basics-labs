#pragma once

namespace Category
{
    enum Type
    {
        None = 0,
        Tractor = 1 << 1,
        Platform = 1 << 2,
        Finish = 1 << 3,
        SoundEffect = 1 << 8,
        Star = 1 << 9,

        CenterOriginSprite = Star,
        CanCollide = Tractor | Finish | Platform | Star,
        Entity = Tractor
    };
}