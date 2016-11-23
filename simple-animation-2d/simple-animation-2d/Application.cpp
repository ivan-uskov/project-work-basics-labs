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
    {
        mShapes.emplace_back(new sf::RectangleShape(sf::Vector2f(120, 50)));
        auto rect = mShapes.back();
        rect->setFillColor(sf::Color::Red);
        rect->setOutlineColor(sf::Color::Black);
        rect->setOutlineThickness(1.f);
        rect->setPosition(200.f, 300.f);
        auto raw = rect.get();
        mAnimations.push_back(Rotor(raw));
    }

    {
        mShapes.emplace_back(new sf::CircleShape(30.f, 30));
        auto circle = new sf::CircleShape(30.f, 30);
        mShapes.emplace_back(circle);
        circle->setFillColor(sf::Color::Yellow);
        circle->setOutlineColor(sf::Color::Black);
        circle->setOutlineThickness(1.f);
        circle->setPosition(mWindow->getSize().x / 2.f, mWindow->getSize().y / 2.f);

        mAnimations.push_back([circle](sf::Time dt) {
            auto pointCount = circle->getPointCount() + 1;
            pointCount = std::max(3u, (pointCount % 30));
            circle->setPointCount(pointCount);
        });
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
