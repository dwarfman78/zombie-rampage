//
// Created by dwarf
//
#include "../include/Application.hpp"
Application::Application(int argc, char **arguments)
    : mWindow(sf::VideoMode(1024, 768), "Zombie Rampage", sf::Style::Default)
{

    mWindow.setFramerateLimit(100);
    std::string arg1;
    if (argc > 1)
    {
        arg1 = {arguments[1]};
    }
    else
    {
        arg1 = "";
    }

    // System order is important !
    if (arg1 == "--debug")
    {
        systems.add<DebugSystem, sf::RenderTarget &>(mWindow);
    }
    systems.add<TopDownRenderingSystem, sf::RenderTarget &>(mWindow);
    systems.add<MovingSystem>();
    systems.add<KeyBoardManagingSystem>(mWindow);
    systems.add<MouseManagingSystem>(mWindow);
    systems.add<CollisionSystem>();
    systems.configure();

    entityx::Entity e = entities.create();
    e.assign<Playable>("Roger");
    e.assign<Renderable>(sf::Vector2f(64 + 16, 64 + 16));
    e.assign<Movable>(sf::Vector2f(0.f, 0.f));

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
