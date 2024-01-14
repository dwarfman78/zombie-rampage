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
    systems.add<ResyncServerSystem>();
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
        acc += dt.asMicroseconds();

        if (acc >= (WINDOW_SIZE))
        {
            // systems.update<ResyncServerSystem>(accMili);
            
      systems.update<MovingSystem>(WINDOW_SIZE_MILLI); 
      systems.update<CollisionSystem>(WINDOW_SIZE_MILLI);
            acc = (acc - WINDOW_SIZE);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        systems.update<NetworkServerSystem>(dt.asMilliseconds());
    }
}
