#include "stdafx.h"
#include "LevelInfo.h"

using namespace std;

void LevelInfo::iterateElements(std::function<void(LevelElementInfo const&)> && fn) const
{
    fn(background);
    for (auto & element : elements)
    {
        fn(element);
    }
    fn(finish);
}

vector<shared_ptr<LevelInfo>> getLevels(std::string const&)
{
    return{
        shared_ptr<LevelInfo>(new LevelInfo{
            "Level 1",
            LevelElementInfo("scene1/Background.png", sf::IntRect(0, 0, 6000, 1000), sf::Vector2f(-500, 0), Category::None, true),
            std::vector<LevelElementInfo>{
                LevelElementInfo("scene1/MainPlatform.png", sf::IntRect(0, 0, 5000, 64), sf::Vector2f(0, 500), Category::Platform, true),
                LevelElementInfo("Textures/Star.png", sf::IntRect(0, 0, 50, 48), sf::Vector2f(2500, 420), Category::Star),
                LevelElementInfo("Textures/Star.png", sf::IntRect(0, 0, 50, 48), sf::Vector2f(2000, 420), Category::Star),
                LevelElementInfo("Textures/Star.png", sf::IntRect(0, 0, 50, 48), sf::Vector2f(3000, 420), Category::Star),
            },
            sf::Vector2f(100, 300),
            LevelElementInfo("Textures/Finish.png", sf::IntRect(0, 0, 63, 84), sf::Vector2f(4900, 420), Category::Finish)
        }),
        shared_ptr<LevelInfo>(new LevelInfo{
            "Level 2",
            LevelElementInfo("scene2/Background.png", sf::IntRect(0, 0, 7000, 3000), sf::Vector2f(-1000, -1000), Category::None, true),
            std::vector<LevelElementInfo>{
                LevelElementInfo("scene1/MainPlatform.png", sf::IntRect(0, 0, 1500, 168), sf::Vector2f(0, 500), Category::Platform, true),
                LevelElementInfo("scene1/MainPlatform.png", sf::IntRect(0, 0, 2000 + 300, 168), sf::Vector2f(1500 - 300, 900), Category::Platform, true),
                LevelElementInfo("scene1/MainPlatform.png", sf::IntRect(0, 0, 1500 + 150, 168), sf::Vector2f(3500 - 150, 1350), Category::Platform, true),
                LevelElementInfo("Textures/Star.png", sf::IntRect(0, 0, 50, 48), sf::Vector2f(1000, 420), Category::Star),
                LevelElementInfo("Textures/Star.png", sf::IntRect(0, 0, 50, 48), sf::Vector2f(1500 - 150, 820), Category::Star),
                LevelElementInfo("Textures/Star.png", sf::IntRect(0, 0, 50, 48), sf::Vector2f(3800 - 100, 1270), Category::Star)
            },
            sf::Vector2f(100, 200),
            LevelElementInfo("Textures/Finish.png", sf::IntRect(0, 0, 76, 200), sf::Vector2f(4900, 1150), Category::Finish)
        })
    };
}