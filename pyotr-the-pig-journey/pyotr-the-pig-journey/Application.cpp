#include "stdafx.h"
#include "Application.h"

#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "TitleState.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "SettingsState.h"
#include "GameOverState.h"
#include "LoadingState.h"

using namespace std;

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
    : mWindow(sf::VideoMode(1024, 768), "Survive Game", sf::Style::Close)
    , mKeyBinding1(1)
    , mKeyBinding2(2)
    , mStateStack(State::Context(mWindow, mTextures, mFonts, mMusic, mSounds, mKeyBinding1, mKeyBinding2))
{
    mWindow.setKeyRepeatEnabled(false);
    mWindow.setVerticalSyncEnabled(true);

    loadCommonResources();
    initializeStatistics();
    setupLoadingState();
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    runAsyncInitialization();

    while (mWindow.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            if (mStateStack.isEmpty())
            {
                mWindow.close();
            }
        }

        updateStatistics(dt);
        render();
    }
}

void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
        {
            mWindow.close();
        }
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);

    mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
    mStatisticsUpdateTime += dt;
    mStatisticsNumFrames += 1;
    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Application::runAsyncInitialization()
{
    mLoadFuture = std::async(std::launch::async, [&] {
        try
        {
            mTextures.load(Textures::Buttons, "Media/Textures/Buttons.png");
            mMusic.setVolume(25.f);
            registerStates();
            mStateStack.initialize();
        }
        catch (std::exception const& e)
        {
            std::cout << e.what() << std::endl;
        }
    });
}

void Application::setupLoadingState()
{
    mStateStack.registerState<LoadingState>(States::Loading);
    mStateStack.pushState(States::Loading);
}

void Application::loadCommonResources()
{
    mFonts.load(Fonts::Main, "Media/Sansation.ttf");
    mTextures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
    mTextures.load(Textures::Preloader, "Media/Textures/Preloader.png");
}

void Application::initializeStatistics()
{
    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(12u);
}

void Application::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<SettingsState>(States::Settings);
    mStateStack.registerState<GameOverState>(States::GameOver, "Mission Failed!");
    mStateStack.registerState<GameOverState>(States::MissionSuccess, "Mission Successful!");
}
