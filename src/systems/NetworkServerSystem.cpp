#include "../../include/systems/NetworkServerSystem.hpp"

void NetworkServerSystem::configure(entityx::EventManager &event_manager)
{
    if (mSocket.bind(3121) != sf::Socket::Done)
    {
        std::cout << "Error" << std::endl;
    }
    else
    {
        mSocket.setBlocking(false);
        mSelector.add(mSocket);
    }
}

void NetworkServerSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    auto fdt = static_cast<float>(dt);
    mCumulDeltaT += fdt;

    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;

    sf::Socket::Status status = mSocket.receive(packet, sender, port);

    if (status == sf::Socket::Status::Done)
    {
        handlePacket(es, packet, sender, port);
    }

    // std::cout << "Last broadcast : " << mLastRun << " Cumul Delta : " << mCumulDeltaT << std::endl;
    if (mCumulDeltaT - mLastRun > 100000)
    {
        mLastRun = mCumulDeltaT;

        broadcastEntityPositions(es);
    }
}
void NetworkServerSystem::broadcastEntityPositions(entityx::EntityManager &es)
{
    for (auto it = mLocalEntitiesToDistantIps.begin(); it != mLocalEntitiesToDistantIps.end(); ++it)
    {
        auto toAdressPort = it->second;

        es.each<Renderable>([&](entityx::Entity entity, Renderable &renderable) {
            sf::Packet packetOut;

            NetworkEvent eventOut;
            eventOut.entityId = entity.id();
            eventOut.type = NetworkEvent::Type::EntityPosition;
            eventOut.entityPosition = renderable.mPos;

            packetOut << eventOut;

            sf::Socket::Status status = mSocket.send(packetOut, std::get<1>(toAdressPort), std::get<2>(toAdressPort));
        });
    }
}
void NetworkServerSystem::handlePacket(entityx::EntityManager &es, sf::Packet &packet, sf::IpAddress &adress,
                                       unsigned short port)
{
    NetworkEvent event;
    packet >> event;

    switch (event.type)
    {
    case NetworkEvent::Type::Connect:
        handleConnect(es, packet, event, adress, port);
        break;
    case NetworkEvent::Type::Disconnect:
        handleDisconnect(es, packet, event, adress, port);
        break;
    case NetworkEvent::Type::Message:
        handleMessage(es, packet, event, adress, port);
        break;
    case NetworkEvent::Type::KeyPressed:
        handleKeyPressed(es, packet, event, adress, port);
        break;
    case NetworkEvent::Type::KeyReleased:
        handleKeyReleased(es, packet, event, adress, port);
        break;
    case NetworkEvent::Type::PlayerAction:
        handlePlayerAction(es, packet, event, adress, port);
        break;
    default:
        handleDefault(es, packet, event, adress, port);
    }
}
void NetworkServerSystem::handlePlayerAction(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                             sf::IpAddress &senderAdress, unsigned short senderPort)
{
    std::cout << "PlayerActionReceived Entity ID " << event.entityId.id()
              << " bitset : " << Tools::actionsToBitset(event.actions).to_string() << " from : " << senderAdress
              << " on port : " << senderPort << std::endl;
    es.each<Actionable, Renderable, Movable>(
        [&](entityx::Entity entity, Actionable &actionable, Renderable &renderable, Movable &movable) {
            if (event.entityId.id() == entity.id().id())
            {
                actionable.actions = event.actions;
            }
        });
}
void NetworkServerSystem::handleKeyPressed(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                           sf::IpAddress &senderAdress, unsigned short senderPort)
{
    std::cout << "KeyPressedReceived " << event.keyCode << " from : " << senderAdress << " on port : " << senderPort
              << std::endl;
}
void NetworkServerSystem::handleKeyReleased(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                            sf::IpAddress &senderAdress, unsigned short senderPort)
{
    std::cout << "KeyReleasedReceived " << event.keyCode << " from : " << senderAdress << " on port : " << senderPort
              << std::endl;
}
void NetworkServerSystem::handleConnect(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                        sf::IpAddress &senderAdress, unsigned short senderPort)
{
    std::cout << "Connection from : " << senderAdress << " on port : " << senderPort << std::endl;

    std::tuple<std::string, sf::IpAddress, unsigned short> senderPortTuple = {event.clientUuid, senderAdress,
                                                                              senderPort};

    entityx::Entity e = es.create();
    e.assign<Playable>("Roger");
    e.assign<Renderable>(sf::Vector2f(128.f, 128.f));
    e.assign<Movable>(sf::Vector2f(0.f, 0.f));
    e.assign<Actionable>();

    broadcastEntityCreation(e);

    std::cout << "Mapping entity id : " << e.id().id() << " to ip : " << senderAdress << " and port : " << senderPort
              << " UUID : " << event.clientUuid << std::endl;

    mLocalEntitiesToDistantIps[e.id()] = senderPortTuple;

    sendAllEntities(es, senderPortTuple);
}
void NetworkServerSystem::sendAllEntities(entityx::EntityManager &es,
                                          const std::tuple<std::string, sf::IpAddress, unsigned short> &to)
{
    es.each<Renderable, Movable>([&](entityx::Entity entity, Renderable &renderable, Movable &movable) {
        sf::Packet packetOut;
        NetworkEvent eventOut;

        eventOut.type = NetworkEvent::Type::EntityCreation;
        eventOut.entityPosition = renderable.mPos;
        eventOut.entityId = entity.id();
        eventOut.clientUuid = std::get<0>(mLocalEntitiesToDistantIps[entity.id()]);
        packetOut << eventOut;

        sf::Socket::Status status = mSocket.send(packetOut, std::get<1>(to), std::get<2>(to));
    });
}
void NetworkServerSystem::broadcastEntityCreation(entityx::Entity &e)
{
    for (auto it = mLocalEntitiesToDistantIps.begin(); it != mLocalEntitiesToDistantIps.end(); ++it)
    {
        sf::Packet packetOut;
        NetworkEvent eventOut;
        auto renderableComponent = e.component<Renderable>();
        auto toAdressPort = it->second;

        eventOut.type = NetworkEvent::Type::EntityCreation;
        eventOut.entityPosition = renderableComponent->mPos;
        eventOut.entityId = e.id();
        eventOut.clientUuid = "";
        packetOut << eventOut;

        sf::Socket::Status status = mSocket.send(packetOut, std::get<1>(toAdressPort), std::get<2>(toAdressPort));
    }
}
void NetworkServerSystem::broadcastEntitySuppression(const entityx::Entity::Id &entityId)
{
    for (auto it = mLocalEntitiesToDistantIps.begin(); it != mLocalEntitiesToDistantIps.end(); ++it)
    {
        sf::Packet packetOut;
        NetworkEvent eventOut;
        auto toAdressPort = it->second;

        eventOut.type = NetworkEvent::Type::EntitySuppression;
        eventOut.entityId = entityId;
        eventOut.clientUuid = "";
        packetOut << eventOut;

        sf::Socket::Status status = mSocket.send(packetOut, std::get<1>(toAdressPort), std::get<2>(toAdressPort));
    }
}
void NetworkServerSystem::handleDisconnect(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                           sf::IpAddress &senderAdress, unsigned short senderPort)
{
    std::cout << "Disconnection from : " << senderAdress << " on port : " << senderPort
              << " Destroying entity : " << event.entityId.id() << std::endl;

    mLocalEntitiesToDistantIps.erase(event.entityId);
    broadcastEntitySuppression(event.entityId);
    es.destroy(event.entityId);
}
void NetworkServerSystem::handleMessage(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                        sf::IpAddress &senderAdress, unsigned short senderPort)
{
    std::cout << "Received : " << event.message << " from : " << senderAdress << " on port : " << senderPort
              << std::endl;
}
void NetworkServerSystem::handleDefault(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                        sf::IpAddress &senderAdress, unsigned short senderPort)
{
    std::cout << "Unknown event from : " << senderAdress << " on port : " << senderPort << std::endl;
}
