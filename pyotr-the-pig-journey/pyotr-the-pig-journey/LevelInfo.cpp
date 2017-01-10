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
            LevelElementInfo("scene1/Background.png", sf::IntRect(0, 0, 7000, 3000), sf::Vector2f(-1000, -1000), Category::None, true),
            std::vector<LevelElementInfo>{
                LevelElementInfo("scene1/MainPlatform.png", sf::IntRect(0, 0, 5000, 168), sf::Vector2f(0, 500), Category::Platform, true),
                LevelElementInfo("Textures/Star.png", sf::IntRect(0, 0, 50, 48), sf::Vector2f(2500, 420), Category::Star),
                LevelElementInfo("Textures/Star.png", sf::IntRect(0, 0, 50, 48), sf::Vector2f(2000, 420), Category::Star),
                LevelElementInfo("Textures/Star.png", sf::IntRect(0, 0, 50, 48), sf::Vector2f(3000, 420), Category::Star)
            },
            sf::Vector2f(100, 200),
            LevelElementInfo("scene1/FinishLine.png", sf::IntRect(0, 0, 76, 200), sf::Vector2f(4900, 300), Category::Finish)
        })
    };
}