#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "ResourceIdentifiers.h"
#include "Category.h"

struct LevelElementInfo
{
    std::string key;
    sf::IntRect rect;
    sf::Vector2f pos;
    Category::Type type;
    bool repeated = false;

    LevelElementInfo(std::string && key, sf::IntRect && rect, sf::Vector2f && pos, Category::Type type, bool repeated = false)
        : key(key)
        , rect(rect)
        , pos(pos)
        , type(type)
        , repeated(repeated)
    {}
};

struct LevelInfo
{
    std::string name;
    LevelElementInfo background;
    std::vector<LevelElementInfo> elements;
    sf::Vector2f startPos;
    LevelElementInfo finish;

    void iterateElements(std::function<void(LevelElementInfo const&)> && fn) const;
};

typedef std::shared_ptr<LevelInfo> LevelPtr;
typedef std::vector<LevelPtr> Levels;

Levels getLevels(std::string const& configFileName);