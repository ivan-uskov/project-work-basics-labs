#pragma once

// Forward declaration of SFML classes
namespace sf
{
    class Texture;
    class Font;
    class Shader;
    class SoundBuffer;
}

namespace Textures
{
    enum ID
    {
        Entities,
        Clouds,
        Grass,
        TitleScreen,
        Buttons,
        Explosion,
        Particle,
        FinishLine,
        ScoreInfo
    };
}

namespace Shaders
{
    enum ID
    {
        BrightnessPass,
        DownSamplePass,
        GaussianBlurPass,
        AddPass,
    };
}

namespace Fonts
{
    enum ID
    {
        Main
    };
}

namespace SoundEffect
{
    enum ID
    {
        AlliedGunfire,
        EnemyGunfire,
        Explosion1,
        Explosion2,
        LaunchMissile,
        CollectPickup,
        CollectStar,
        Button,
    };
}

namespace Music
{
    enum ID
    {
        MenuTheme,
        MissionTheme,
    };
}

//forward decl
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;
typedef ResourceHolder<sf::Texture, std::string> LevelTextureHolder;
typedef std::shared_ptr<LevelTextureHolder> LevelTexturesPtr;