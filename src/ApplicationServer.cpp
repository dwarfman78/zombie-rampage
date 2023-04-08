//
// Created by dwarf
//
#include "../include/ApplicationServer.hpp"
ApplicationServer::ApplicationServer(int argc, char **arguments)
{
}
void ApplicationServer::start()
{
    systems.add<MovingSystem>();
    systems.add<CollisionSystem>();
    systems.add<NetworkServerSystem>();
    systems.configure();

    entityx::Entity map = entities.create();
    map.assign<TileMap>();

    std::cout << "Application server started !" << std::endl;

    sf::Clock deltaClock;
    sf::Int32 acc = 0;
    sf::Int32 tickRate = 64;
    sf::Int32 wSize = 1000000 / tickRate;
    while (true)
    {
        sf::Time dt = deltaClock.restart();
        acc += dt.asMicroseconds();

        if (acc >= (wSize))
        {
            systems.update<MovingSystem>(acc / 1000);
            systems.update<CollisionSystem>(acc / 1000);
            acc = -(acc - wSize);
        }

        systems.update<NetworkServerSystem>(dt.asMicroseconds());
    }
}