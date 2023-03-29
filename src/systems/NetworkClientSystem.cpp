#include "../../include/systems/NetworkClientSystem.hpp"

void NetworkClientSystem::configure(entityx::EventManager &event_manager)
{
    mSocket.setBlocking(false);

    event_manager.subscribe<sf::Event>(*this);
}

void NetworkClientSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{

    if (mStatus == Disconnected && !mDying)
    {
        sendConnection();
    }
    else
    {
        auto fdt = static_cast<float>(dt);
        mCumulDeltaT += fdt;

        sf::Packet packet;
        sf::IpAddress sender;
        unsigned short port;

        sf::Socket::Status status = mSocket.receive(packet, sender, port);

        if (status == sf::Socket::Done)
        {
            handlePacket(packet, es, events);
        }

        if (mCumulDeltaT - mLastRun > 5000)
        {
            mLastRun = mCumulDeltaT;

            // sendHeartBeat();
        }
    }
}
void NetworkClientSystem::handlePacket(sf::Packet &packet, entityx::EntityManager &entities,
                                       entityx::EventManager &events)
{
    NetworkEvent incomingEvent;

    packet >> incomingEvent;

    if (incomingEvent.type == NetworkEvent::Type::EntityCreation)
    {

        std::cout << " Received EntityCreation : Entity Id : " << incomingEvent.entityId.id()
                  << " UUID : " << incomingEvent.clientUuid << " entityPosition : " << incomingEvent.entityPosition.x
                  << ", " << incomingEvent.entityPosition.y << std::endl;

        entityx::Entity e = entities.create();
        e.assign<Playable>("Roger");
        e.assign<Renderable>(incomingEvent.entityPosition);

        if (mStatus == Pending && incomingEvent.clientUuid == mUuid)
        {
            e.assign<Movable>(sf::Vector2f(0.f, 0.f));
            mStatus = Connected;
            std::cout << "Local personnal id : " << e.id().id() << std::endl;
            mId = e.id();
        }

        std::cout << "Mapping local entity id : " << e.id().id()
                  << " to distant entity : " << incomingEvent.entityId.id() << std::endl;
        mDistantToLocalEntitiesIds[incomingEvent.entityId] = e.id();
        mLocalToDistantEntitiesIds[e.id()] = incomingEvent.entityId;
    }
    else if (incomingEvent.type == NetworkEvent::Type::EntitySuppression)
    {
        std::cout << "Received EntitySuppression : EntityId : " << incomingEvent.entityId.id() << std::endl;
        auto localId = mDistantToLocalEntitiesIds[incomingEvent.entityId];
        mLocalToDistantEntitiesIds.erase(localId);
        mDistantToLocalEntitiesIds.erase(incomingEvent.entityId);
        entities.destroy(incomingEvent.entityId);
    }

    events.emit<NetworkEvent>(incomingEvent);
}
void NetworkClientSystem::receive(const sf::Event &event)
{
    if (event.type == sf::Event::Closed ||
        ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
    {
        sendDisconnection(mLocalToDistantEntitiesIds[mId]);
        mDying = true;
    }
}
void NetworkClientSystem::sendHeartBeat()
{
    sf::Packet packet;
    sf::IpAddress server = {"127.0.0.1"};
    unsigned short port = 3121;
    NetworkEvent event;

    event.type = NetworkEvent::Type::Message;
    event.message = "Hi !";

    packet << event;

    sf::Socket::Status status = mSocket.send(packet, server, port);
}
void NetworkClientSystem::sendDisconnection(entityx::Entity::Id id)
{
    sf::Packet packet;
    sf::IpAddress server = {"127.0.0.1"};
    unsigned short port = 3121;
    NetworkEvent event;

    event.type = NetworkEvent::Type::Disconnect;
    event.entityId = id;

    std::cout << "Sending disconnect : " << event.entityId.id() << std::endl;

    packet << event;

    sf::Socket::Status status = mSocket.send(packet, server, port);

    mStatus = Disconnected;
}
void NetworkClientSystem::sendConnection()
{
    sf::Packet packet;
    sf::IpAddress server = {"127.0.0.1"};
    unsigned short port = 3121;
    NetworkEvent event;
    event.clientUuid = Tools::uuidFromTimestamp();
    mUuid = event.clientUuid;

    std::cout << "Sending connection with uuid : " << mUuid << std::endl;

    event.type = NetworkEvent::Type::Connect;

    packet << event;

    sf::Socket::Status status = mSocket.send(packet, server, port);

    mStatus = Pending;
}