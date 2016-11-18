#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "ISceneNode.h"
#include "Renderer.h"

class Application
{
public:
    Application();
    void run();

private:
    void initializeWindow();
    void initializeResources();
    void initializeStatistics();
    void initializeScene();

    void updateStatistics(sf::Time dt);
    void processInput();

    void setupCamera();

    void updateModel(sf::Clock & clock, sf::Time & timeSinceLastUpdate);
    void render();

private:
    const sf::Time TimePerFrame;

    std::unique_ptr<sf::RenderWindow> mWindow;

    std::unique_ptr<ISceneNode> mModel;
    Renderer mView;

    sf::Font mFont;
    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    size_t mStatisticsNumFrames = 0;
};
