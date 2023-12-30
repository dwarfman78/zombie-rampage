//
// Created by dwarf
//
#include "../include/ApplicationServer.hpp"
ApplicationServer::ApplicationServer(int argc, char **arguments)
{
}
void ApplicationServer::start()
{
    systems.add<MovingSystem>(true);
    systems.add<CollisionSystem>();
    systems.add<NetworkServerSystem>();
    systems.configure();

    entityx::Entity map = entities.create();
    map.assign<TileMap>();

    std::cout << "Application server started !" << std::endl;

    sf::Clock deltaClock;
    sf::Int32 acc = 0;
    bool runServer = true;
    while (runServer)
    {
        sf::Time dt = deltaClock.restart();
        if (dt.asMicroseconds() == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        else
        {
            acc += dt.asMicroseconds();
        }

        if (acc >= (WINDOW_SIZE))
        {
            std::cout << "Tick " << acc << std::endl;
            systems.update<MovingSystem>(acc / 1000.f);
            systems.update<CollisionSystem>(acc / 1000.f);
            acc = (acc - WINDOW_SIZE);
        }

        systems.update<NetworkServerSystem>(dt.asMilliseconds());
    }
}
