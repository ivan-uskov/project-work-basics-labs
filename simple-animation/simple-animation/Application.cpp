#include "stdafx.h"
#include "Application.h"

#include "Rotor.h"
#include "CubeNode.h"

#include <SFML/Window/Event.hpp>
#include <gl/GLU.h>

using namespace std;

Application::Application()
    : TimePerFrame(sf::seconds(1.f / 60.f))
{
    initializeWindow();
    initializeResources();
    initializeStatistics();
    initializeScene();
}

void Application::initializeResources()
{
    mFont.loadFromFile("Media/font/Sansation.ttf");
}

void Application::initializeStatistics()
{
    mStatisticsText.setFont(mFont);
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(12u);
    mStatisticsText.setFillColor(sf::Color::Black);
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
    while (timeSinceLastUpdate > TimePerFrame)
    {
        processInput();
        timeSinceLastUpdate -= TimePerFrame;
        mModel->update(dt);
    }

    updateStatistics(dt);
}

void Application::render()
{
    mWindow->clear(sf::Color::White);

    setupCamera();

    mView.render(*mModel);

    mWindow->pushGLStates();
    mWindow->draw(mStatisticsText);
    mWindow->popGLStates();

    mWindow->display();
}

void Application::setupCamera()
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(mWindow->getSize().y / 45.0, 1.0f * mWindow->getSize().x / mWindow->getSize().y, 0.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    gluLookAt(0, 50, 500, 0, 0, 0, 0, 1, 0);

    glColor3f(0.0, 0.0, 0.0);
    glScalef(1, -1, 1);
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
        else if (event.type == sf::Event::Resized)
        {
            // adjust the viewport when the window is resized
            glViewport(0, 0, event.size.width, event.size.height);
        }
    }
}

void Application::initializeScene()
{
    auto view = make_unique<CubeNode>(glm::vec3(40, 40, -50), Cube(glm::vec3(40, 40, -50), 20));
    view->addTransformation(make_unique<Rotor>(1, glm::vec3(0, 0, -50), glm::vec3(0, 1, 0)));
    mModel = move(view);
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

void Application::initializeWindow()
{
    // create the window
    mWindow = make_unique<sf::RenderWindow>(sf::VideoMode(1024, 768), "Animation", sf::Style::Default);
    mWindow->setVerticalSyncEnabled(true);
}