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
    while (true)
    {
        sf::Time dt = deltaClock.restart();
        systems.update_all(dt.asMilliseconds());
    }
}