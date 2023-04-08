//
// Created by dwarf
//
#include "../include/Application.hpp"
Application::Application(int argc, char **arguments)
    : mWindow(sf::VideoMode(1024, 768), "Zombie Rampage", sf::Style::Default)
{

    // mWindow.setFramerateLimit(100);
    std::string lastArg;
    if (argc > 1)
    {
        lastArg = {arguments[argc - 1]};
    }
    else
    {
        lastArg = "";
    }

    // System order is important !
    if (lastArg == "--debug")
    {
        systems.add<DebugSystem, sf::RenderTarget &>(mWindow);
    }
    systems.add<TopDownRenderingSystem, sf::RenderTarget &>(mWindow);
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
    sf::Int32 tickRate = 64;
    sf::Int32 wSize = 1000000 / tickRate;
    sf::Int32 graphicLoops = 0;
    sf::Int32 logicLoops = 0;
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

        // std::cout << "Updating graphics" << graphicLoops << std::endl;
        graphicLoops++;
        if (acc >= (wSize))
        {
            // std::cout << "Updating logic" << logicLoops << std::endl;
            logicLoops++;
            systems.update<MovingSystem>(acc / 1000);
            systems.update<CollisionSystem>(acc / 1000);
            acc = -(acc - wSize);
        }

        systems.update<KeyBoardManagingSystem>(dt.asMilliseconds());
        systems.update<MouseManagingSystem>(dt.asMilliseconds());
        systems.update<NetworkClientSystem>(dt.asMilliseconds());

        mWindow.display();
    }
}
