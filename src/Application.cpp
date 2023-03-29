//
// Created by dwarf
//
#include "../include/Application.hpp"
Application::Application(int argc, char **arguments)
    : mWindow(sf::VideoMode(1024, 768), "Zombie Rampage", sf::Style::Default)
{

    mWindow.setFramerateLimit(100);
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
    while (mWindow.isOpen())
    {
        sf::Event event;

        while (mWindow.pollEvent(event))
        {
            events.emit<sf::Event>(event);
        }
        mWindow.clear();

        sf::Time dt = deltaClock.restart();

        systems.update_all(dt.asMilliseconds());

        mWindow.display();
    }
}
