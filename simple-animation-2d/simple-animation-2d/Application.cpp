#include "stdafx.h"
#include "Application.h"
#include "Animations.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

Application::Application()
    : TIME_PER_FRAME(sf::seconds(1.f / 60.f))
{
    initializeWindow();
    initializeScene();
    initializeResources();
    initializeStatistics();
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow->isOpen())
    {
        updateModel(clock, timeSinceLastUpdate);
        render();
    }
}

void Application::updateModel(sf::Clock & clock, sf::Time & timeSinceLastUpdate)
{
    sf::Time dt = clock.restart();
    timeSinceLastUpdate += dt;
    while (timeSinceLastUpdate > TIME_PER_FRAME)
    {
        processInput();
        timeSinceLastUpdate -= TIME_PER_FRAME;

        for (auto const& animation : mAnimations)
        {
            animation(dt);
        }
    }

    updateStatistics(dt);
}

void Application::render()
{
    mWindow->clear(sf::Color::White);

    for (auto const& shapePtr : mShapes)
    {
        mWindow->draw(*shapePtr);
    }

    mWindow->draw(mStatisticsText);

    mWindow->display();
}


void Application::processInput()
{
    sf::Event event;
    while (mWindow->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            mWindow->close();
        }
    }
}

void Application::updateStatistics(sf::Time dt)
{
    mStatisticsUpdateTime += dt;
    mStatisticsNumFrames += 1;
    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Application::initializeScene()
{
    auto window = mWindow.get();
    auto const rectsCount = 100;
    auto const rectSize = sf::Vector2f(50, 40);
    auto const rectDistance = 20.f;
    auto const colors = std::array<sf::Color, 5>({
        sf::Color::Red, sf::Color::Yellow, sf::Color::Blue, sf::Color::Black, sf::Color::Green
    });

    for (int i = 0; i < rectsCount; ++i)
    {
        mShapes.emplace_back(new sf::RectangleShape(rectSize));
        auto rect = mShapes.back();
        rect->setFillColor(colors[i % colors.size()]);
        rect->setOutlineColor(sf::Color::Black);
        rect->setOutlineThickness(1.f);
        rect->setPosition(
            window->getSize().x / 2.f ,
            float(static_cast<int>(rectDistance + (i * (rectDistance + rectSize.y))) % window->getSize().y)
        );
        auto raw = rect.get();
        mAnimations.push_back(Rotor(raw, intToDirection(i), i * 30.f));
        mAnimations.push_back(ColorBlinker(raw, intToDirection(i), i + 1));
        mAnimations.push_back(Scaler(raw, window, 0.002f * ((i + 1) % 20)));
        mAnimations.push_back(Mover(raw, window, float((i + 1) % 10)));
    }
}

void Application::initializeResources()
{
    mFont.loadFromFile("Media/font/Sansation.ttf");
}

void Application::initializeWindow()
{
    mWindow = make_unique<sf::RenderWindow>(sf::VideoMode(1024, 768), "Animation", sf::Style::Default);
    mWindow->setVerticalSyncEnabled(true);
}

void Application::initializeStatistics()
{
    mStatisticsText.setFont(mFont);
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(12u);
    mStatisticsText.setFillColor(sf::Color::Black);
}
