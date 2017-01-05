#include "stdafx.h"
#include "LevelInfo.h"

using namespace std;

void LevelInfo::iterateElements(std::function<void(LevelElementInfo const&)> && fn) const
{
    fn(background);
    for (auto & platform : platforms)
    {
        fn(platform);
    }
    fn(finish);
}

vector<shared_ptr<LevelInfo>> getLevels(std::string const&)
{
    return{
        shared_ptr<LevelInfo>(new LevelInfo{
            "Simple Scene",
            LevelElementInfo("scene1/Background.png", sf::IntRect(0, 0, 5000 + 1024,  768 + 768), sf::Vector2f(-512, -768 / 2), Category::None, true),
            std::vector<LevelElementInfo>{
                LevelElementInfo("scene1/MainPlatform.png", sf::IntRect(0, 0, 5000, 168), sf::Vector2f(0, 500), Category::Platform, true)
            },
            sf::Vector2f(100, 200),
            LevelElementInfo("scene1/FinishLine.png", sf::IntRect(0, 0, 76, 500), sf::Vector2f(4900, 0), Category::Finish)
        })
    };
}