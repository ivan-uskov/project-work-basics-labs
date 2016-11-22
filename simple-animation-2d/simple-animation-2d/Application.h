#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>

class Application
{
public:
    Application();

    void run();

private:
    void initializeWindow();
    void initializeScene();
    void initializeResources();
    void initializeStatistics();
    void updateStatistics(sf::Time dt);
    void processInput();

    void updateModel(sf::Clock & clock, sf::Time & timeSinceLastUpdate);
    void render();

private:
    const sf::Time TIME_PER_FRAME;

    std::unique_ptr<sf::RenderWindow> mWindow;
    std::vector<std::shared_ptr<sf::Shape>> mShapes;
    std::vector<std::function<void(sf::Time dt)>> mAnimations;

    sf::Font mFont;
    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    size_t mStatisticsNumFrames = 0;
};

