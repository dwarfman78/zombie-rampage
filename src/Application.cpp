//
// Created by dwarf
//
#include "../include/Application.hpp"
#include <SFML/Window/WindowStyle.hpp>
Application::Application(int argc, char **arguments)
    : mWindow(sf::VideoMode(1920,1080), "Zombie Rampage", sf::Style::Fullscreen)
{

    std::string lastArg;
    if (argc > 1)
    {
        lastArg = arguments[argc - 1];
    }
    else
    {
        lastArg = "";
    }

    // System order is important !
    if (lastArg == "--debug")
    {
        mDebug = true;
        systems.add<DebugSystem, sf::RenderTarget &>(mWindow);
    }
    systems.add<TopDownRenderingSystem, sf::RenderTarget &>(mWindow);
    systems.add<InterpolationSystem>();
    systems.add<MovingSystem>();
    systems.add<KeyBoardManagingSystem>(mWindow);
    systems.add<MouseManagingSystem>(mWindow);
    systems.add<CollisionSystem>();
    systems.add<NetworkClientSystem>();
    systems.configure();

    entityx::Entity map = entities.create();
    map.assign<TileMap>();
}

void Application::start()
{

    std::cout << "Application started !" << std::endl;

    sf::Clock deltaClock;
    sf::Int32 acc = 0;

    while (mWindow.isOpen())
    {
        sf::Time dt = deltaClock.restart();
        acc += dt.asMicroseconds();

        sf::Event event;

        while (mWindow.pollEvent(event))
        {
            events.emit<sf::Event>(event);
        }

        mWindow.clear();
        systems.update<TopDownRenderingSystem>(dt.asMilliseconds());

        if (acc >= (WINDOW_SIZE))
        {
            systems.update<InterpolationSystem>(WINDOW_SIZE_MILLI);
            systems.update<MovingSystem>(WINDOW_SIZE_MILLI);
            systems.update<CollisionSystem>(WINDOW_SIZE_MILLI);
            acc = (acc - WINDOW_SIZE);
        }

        systems.update<KeyBoardManagingSystem>(dt.asMilliseconds());
        systems.update<MouseManagingSystem>(dt.asMilliseconds());
        systems.update<NetworkClientSystem>(dt.asMilliseconds());

        if (mDebug)
        {
            systems.update<DebugSystem>(dt.asMilliseconds());
        }

        mWindow.display();
    }
}
