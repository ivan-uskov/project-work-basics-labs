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

    {
        mShapes.emplace_back(new sf::RectangleShape(sf::Vector2f(100, 80)));
        auto rect = mShapes.back();
        rect->setFillColor(sf::Color::Red);
        rect->setOutlineColor(sf::Color::Black);
        rect->setOutlineThickness(1.f);
        rect->setPosition(window->getSize().x / 2.f , 100.f);
        auto raw = rect.get();
        mAnimations.push_back(Rotor(raw));
        mAnimations.push_back([raw](sf::Time dt) {
            auto color = raw->getFillColor();
            color.a += 1;
            raw->setFillColor(color);
        });
        mAnimations.push_back([raw, window](sf::Time dt) {
            auto coef = 0.005f;
            auto pos = raw->getPosition();
            if (pos.x <= window->getSize().x / 2 && pos.y < window->getSize().y / 2)
            {
                auto scale = raw->getScale();
                scale.x -= coef;
                scale.y += coef;
                raw->setScale(scale);
            }
            else if (pos.x <= window->getSize().x / 2 && pos.y >= window->getSize().y / 2)
            {
                auto scale = raw->getScale();
                scale.x += coef;
                scale.y += coef;
                raw->setScale(scale);
            }
            else if (pos.x > window->getSize().x / 2 && pos.y <= window->getSize().y / 2)
            {
                auto scale = raw->getScale();
                scale.x -= coef;
                scale.y -= coef;
                raw->setScale(scale);
            }
            else if (pos.x > window->getSize().x / 2 && pos.y >= window->getSize().y / 2)
            {
                auto scale = raw->getScale();
                scale.x += coef;
                scale.y -= coef;
                raw->setScale(scale);
            }
        });
        mAnimations.push_back([raw, window](sf::Time dt) {
            auto pos = raw->getPosition();
            if (pos.x <= window->getSize().x / 2 && pos.y < window->getSize().y / 2)
            {
                raw->move(-1.f, 1.f);
            }
            else if (pos.x <= window->getSize().x / 2 && pos.y >= window->getSize().y / 2)
            {
                raw->move(1.f, 1.f);
            }
            else if (pos.x > window->getSize().x / 2 && pos.y <= window->getSize().y / 2)
            {
                raw->move(-1.f, -1.f);
            }
            else if (pos.x > window->getSize().x / 2 && pos.y >= window->getSize().y / 2)
            {
                raw->move(1.f, -1.f);
            }
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
