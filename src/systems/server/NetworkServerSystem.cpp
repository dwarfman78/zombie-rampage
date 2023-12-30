#include "../../../include/systems/server/NetworkServerSystem.hpp"

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
    //  100000
    if (mCumulDeltaT - mLastRun > 1000)
    {
        mLastRun = mCumulDeltaT;

        broadcastWorldState(es);
    }
    mServerTickNumber++;
}
void NetworkServerSystem::broadcastWorldState(entityx::EntityManager &es)
{
    NetworkEvent eventOut;

    prepareWorldStateEvent(es, eventOut);

    for (auto it = mLocalEntitiesToDistantIps.begin(); it != mLocalEntitiesToDistantIps.end(); ++it)
    {
        auto toAdressPort = it->second;

        es.each<Renderable, Movable>([&](entityx::Entity entity, Renderable &renderable, Movable &movable) {
            sf::Packet packetOut;
            packetOut << eventOut;

            sf::Socket::Status status = mSocket.send(packetOut, std::get<1>(toAdressPort), std::get<2>(toAdressPort));
        });
    }
}
void NetworkServerSystem::prepareWorldStateEvent(entityx::EntityManager &es, NetworkEvent &eventOut)
{
    eventOut.serverTick = mServerTickNumber;
    eventOut.type = NetworkEvent::Type::WorldState;
    es.each<Renderable, Movable, Networkable>(
        [&](entityx::Entity entity, Renderable &renderable, Movable &movable, Networkable &networkable) {
            eventOut.worldState[entity.id()] = {renderable.mPos, movable.mAcceleration, networkable};
        });
}
void NetworkServerSystem::handlePacket(entityx::EntityManager &es, sf::Packet &packet, sf::IpAddress &adress,
                                       unsigned short port)
{
    NetworkEvent event;
    packet >> event;

    event.latency = Tools::currentTimestamp() - event.currentTimestamp;

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
    case NetworkEvent::Type::Ping:
        handlePing(es, packet, event, adress, port);
        break;
    default:
        handleDefault(es, packet, event, adress, port);
    }
}
void NetworkServerSystem::handlePing(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                     sf::IpAddress &senderAdress, unsigned short senderPort)
{
}
void NetworkServerSystem::handlePlayerAction(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                             sf::IpAddress &senderAdress, unsigned short senderPort)
{
    std::cout << "PlayerActionReceived Entity ID " << event.serverEntityId.id()
              << " bitset : " << Tools::actionsToBitset(event.actions).to_string() << " from : " << senderAdress
              << " on port : " << senderPort << " Latency : " << event.latency << std::endl;

    auto entity = es.get(event.serverEntityId);
    if (entity.valid())
    {
        auto actionable = es.component<Actionable>(event.serverEntityId);
        auto networkable = es.component<Networkable>(event.serverEntityId);

        networkable->clientServerDelay = event.latency;
        actionable->actions = event.actions;
        networkable->isDesync = true;
    }
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
    e.assign<Networkable>();

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
        eventOut.serverEntityId = entity.id();
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
        eventOut.serverEntityId = e.id();
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
        eventOut.serverEntityId = entityId;
        eventOut.clientUuid = "";
        packetOut << eventOut;

        sf::Socket::Status status = mSocket.send(packetOut, std::get<1>(toAdressPort), std::get<2>(toAdressPort));
    }
}
void NetworkServerSystem::handleDisconnect(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                                           sf::IpAddress &senderAdress, unsigned short senderPort)
{
    std::cout << "Disconnection from : " << senderAdress << " on port : " << senderPort
              << " Destroying entity : " << event.serverEntityId.id() << std::endl;

    mLocalEntitiesToDistantIps.erase(event.serverEntityId);
    broadcastEntitySuppression(event.serverEntityId);
    es.destroy(event.serverEntityId);
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
