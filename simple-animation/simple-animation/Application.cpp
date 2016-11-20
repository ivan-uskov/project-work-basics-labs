#include "stdafx.h"
#include "Application.h"

#include "Rotor.h"
#include "Colorer.h"
#include "CubeNode.h"
#include "SphereNode.h"

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

void setOpenGLMatrix(const float *matrix, GLint mode)
{
    GLint oldMode = 0;
    glGetIntegerv(GL_MATRIX_MODE, &oldMode);
    if (oldMode != mode)
    {
        glMatrixMode(mode);
        glLoadMatrixf(matrix);
        glMatrixMode(oldMode);
    }
    else
    {
        glLoadMatrixf(matrix);
    }
}


void Application::setupCamera()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(0.1f);
    glEnable(GL_LINE_SMOOTH);

    using namespace glm;
    auto viewport = mWindow->getView().getViewport();
    auto perspective = glm::perspective(80.f, float(viewport.width) / viewport.height, 0.f, 100.f);
    setOpenGLMatrix(glm::value_ptr(perspective), GL_PROJECTION);

    auto modelView = lookAt(vec3(0, 10.f, 200.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));
    setOpenGLMatrix(value_ptr(modelView), GL_MODELVIEW);
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
            glViewport(0, 0, event.size.width, event.size.height);
        }
    }
}

void Application::initializeScene()
{
    {
        auto view = make_unique<SphereNode>(glm::vec3(0, 0, 0), Sphere(50));
        view->addTransformation(make_unique<Rotor>(2000, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
        view->addTransformation(make_unique<Colorer>(vector<glm::u8vec4>{
            { 255, 0, 0, 255 },
            { 255, 255, 0, 255 },
            { 0, 255, 0, 255 },
            { 255, 0, 255, 255 },
            { 255, 255, 255, 255 },
            { 0, 255, 255, 255 },
            { 0, 0, 255, 255 }
        }));
        mModel = move(view);
    }

    {
        auto view = make_unique<CubeNode>(glm::vec3(9, 50, -90), Cube(25));
        view->addTransformation(make_unique<Rotor>(4000, glm::vec3(9, 50, -90), glm::vec3(1, 1, -1)));
        view->addTransformation(make_unique<Rotor>(4000, glm::vec3(0, 0, 0), glm::vec3(-1, 1, -1)));
        view->addTransformation(make_unique<Rotor>(4000, glm::vec3(40, 0, 0), glm::vec3(1, 1, 1)));
        view->addTransformation(make_unique<Rotor>(3000, glm::vec3(0, 0, 20), glm::vec3(-1, 0, -1)));
        mModel->addChild(move(view));
    }

    {
        auto view = make_unique<CubeNode>(glm::vec3(9, 90, -90), Cube(35));
        view->addTransformation(make_unique<Rotor>(3500, glm::vec3(9, 90, -90), glm::vec3(1, 0, -1)));
        view->addTransformation(make_unique<Rotor>(4500, glm::vec3(0, 0, 0), glm::vec3(-1, -1, -1)));
        view->addTransformation(make_unique<Rotor>(3500, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
        view->addTransformation(make_unique<Rotor>(2500, glm::vec3(0, 0, 0), glm::vec3(1, 0, 1)));
        mModel->addChild(move(view));
    }

    {
        auto view = make_unique<CubeNode>(glm::vec3(90, 0, 0), Cube(35));
        view->addTransformation(make_unique<Rotor>(3500, glm::vec3(-90, 0, 0), glm::vec3(1, 0, -1)));
        view->addTransformation(make_unique<Rotor>(4000, glm::vec3(0, 0, 0), glm::vec3(-1, -1, -1)));
        mModel->addChild(move(view));
    }

    {
        auto view = make_unique<CubeNode>(glm::vec3(90, 0, 0), Cube(30));
        view->addTransformation(make_unique<Rotor>(2000, glm::vec3(90, 0, 0), glm::vec3(1, 0, 0)));
        view->addTransformation(make_unique<Rotor>(3000, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
        view->addTransformation(make_unique<Rotor>(4000, glm::vec3(0, 0, 0), glm::vec3(0, 1, 1)));
        mModel->addChild(move(view));
    }

    {
        auto view = make_unique<CubeNode>(glm::vec3(80, 0, 0), Cube(20));
        view->addTransformation(make_unique<Rotor>(5000, glm::vec3(80, 0, 0), glm::vec3(0, 0, 1)));
        view->addTransformation(make_unique<Rotor>(3000, glm::vec3(0, 0, 0), glm::vec3(1, 1, 0)));
        mModel->addChild(move(view));
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

void Application::initializeWindow()
{
    mWindow = make_unique<sf::RenderWindow>(sf::VideoMode(1024, 768), "Animation", sf::Style::Default);
    mWindow->setVerticalSyncEnabled(true);
}