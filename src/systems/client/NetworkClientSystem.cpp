#include "../../../include/systems/client/NetworkClientSystem.hpp"
#include <entityx/Entity.h>

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

            sendHeartBeat();
        }
        if (mStatus == Connected)
        {
            auto self = es.get(mId);
            if (self.valid() && self.has_component<Networkable>() && self.has_component<Actionable>())
            {
                const auto &actionable = self.component<Actionable>();
                const auto &networkable = self.component<Networkable>();
                auto bitsetActions = Tools::actionsToBitset(actionable->actions);
                if (bitsetActions != mLastSentActions)
                {
                    sendActions(actionable->actions);
                    mLastSentActions = bitsetActions;
                }
            }
        }
    }
}
void NetworkClientSystem::handlePacket(sf::Packet &packet, entityx::EntityManager &entities,
                                       entityx::EventManager &events)
{
    NetworkEvent incomingEvent;

    packet >> incomingEvent;

    switch (incomingEvent.type)
    {
    case NetworkEvent::Type::EntityCreation:
        handleEntityCreation(entities, incomingEvent);
        break;
    case NetworkEvent::Type::EntitySuppression:
        handleEntitySuppression(entities, incomingEvent);
        break;
    case NetworkEvent::Type::WorldState:
        handleWorldState(entities, incomingEvent);
        break;
    default:;
        break;
    }

    events.emit<NetworkEvent>(incomingEvent);
}
void NetworkClientSystem::handleWorldState(entityx::EntityManager &entities, NetworkEvent &incomingEvent)
{
    if (incomingEvent.serverTick > mLastServerTick)
    {
        for (auto &[entityId, state] : incomingEvent.worldState)
        {
            auto &localId = mDistantToLocalEntitiesIds[entityId];

            // if (localId != mId)
            {
                auto entity = entities.get(localId);
                if (entity.valid() && entity.has_component<Networkable>())
                {
                    auto &[serverRenderable, serverMovable, serverNetworkable] = state;
                    auto networkable = entity.component<Networkable>();
                    networkable->pushState(
                        {incomingEvent.serverTick, serverRenderable, serverMovable, serverNetworkable});
                }
            }
        }
        mLastServerTick = incomingEvent.serverTick;
    }
}
void NetworkClientSystem::handleEntityPosition(entityx::EntityManager &entities, NetworkEvent &incomingEvent)
{
    std::cout << "Received EntityPosition : EntityId : " << incomingEvent.serverEntityId.id() << " new Pos : ( "
              << incomingEvent.entityPosition.x << " , " << incomingEvent.entityPosition.y << " ) " << std::endl;

    entities.each<Renderable>([&](entityx::Entity entity, Renderable &renderable) {
        auto localIncomingEntityId = mDistantToLocalEntitiesIds[incomingEvent.serverEntityId];

        if (entity.id() == localIncomingEntityId && entity.id() != mId)
        {
            renderable.mPos = incomingEvent.entityPosition;
        }
    });
}
void NetworkClientSystem::handleEntitySuppression(entityx::EntityManager &entities, NetworkEvent &incomingEvent)
{
    std::cout << "Received EntitySuppression : EntityId : " << incomingEvent.serverEntityId.id() << std::endl;
    auto localId = mDistantToLocalEntitiesIds[incomingEvent.serverEntityId];
    mLocalToDistantEntitiesIds.erase(localId);
    mDistantToLocalEntitiesIds.erase(incomingEvent.serverEntityId);
    entities.destroy(localId);
}
void NetworkClientSystem::handleEntityCreation(entityx::EntityManager &entities, NetworkEvent &incomingEvent)
{
    std::cout << " Received EntityCreation : Entity Id : " << incomingEvent.serverEntityId.id()
              << " UUID : " << incomingEvent.clientUuid << " entityPosition : " << incomingEvent.entityPosition.x
              << ", " << incomingEvent.entityPosition.y << std::endl;

    entityx::Entity e = entities.create();
    e.assign<Renderable>(incomingEvent.entityPosition);
    e.assign<Movable>(sf::Vector2f(0.f, 0.f));
    e.assign<Actionable>();
    e.assign<Animable>("player");

    if (mStatus == Pending && incomingEvent.clientUuid == mUuid)
    {
        e.assign<Playable>("Roger");
        mStatus = Connected;
        std::cout << "Local personnal id : " << e.id().id() << std::endl;
        mId = e.id();
    }

    e.assign<Networkable>(e.id(), incomingEvent.serverEntityId, 0);

    std::cout << "Mapping local entity id : " << e.id().id()
              << " to distant entity : " << incomingEvent.serverEntityId.id() << std::endl;
    mDistantToLocalEntitiesIds[incomingEvent.serverEntityId] = e.id();
    mLocalToDistantEntitiesIds[e.id()] = incomingEvent.serverEntityId;
    incomingEvent.clientEntityId = e.id();
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
void NetworkClientSystem::sendEventToServer(const NetworkEvent &event)
{
    sf::Packet packet = Tools::preparePacket(mServerIp, mServerPort);

    packet << event;
    sf::Socket::Status status = mSocket.send(packet, mServerIp, mServerPort);

    if (status != sf::Socket::Status::Done)
    {
        std::cout << "WARNING : cannot send packet... " << std::endl;
    }
}
void NetworkClientSystem::sendActions(const std::map<Actionable::Action, bool> &actions)
{
    NetworkEvent event;
    event.type = NetworkEvent::Type::PlayerAction;
    event.serverEntityId = mLocalToDistantEntitiesIds[mId];
    event.actions = actions;

    std::cout << "Sending Actions : " << Tools::actionsToBitset(actions).to_string()
              << " Entity ID : " << event.serverEntityId.id() << std::endl;

    sendEventToServer(event);
}
void NetworkClientSystem::sendKeyReleased(entityx::Entity::Id id, const sf::Keyboard::Key &key)
{
    NetworkEvent event;
    event.type = NetworkEvent::Type::KeyReleased;
    event.keyCode = key;

    sendEventToServer(event);
}
void NetworkClientSystem::sendKeyPressed(entityx::Entity::Id id, const sf::Keyboard::Key &key)
{
    NetworkEvent event;
    event.type = NetworkEvent::Type::KeyPressed;
    event.keyCode = key;

    sendEventToServer(event);
}
void NetworkClientSystem::sendHeartBeat()
{
    NetworkEvent event;
    event.type = NetworkEvent::Type::Ping;

    sendEventToServer(event);
}
void NetworkClientSystem::sendDisconnection(entityx::Entity::Id id)
{
    NetworkEvent event;
    event.type = NetworkEvent::Type::Disconnect;
    event.serverEntityId = id;

    std::cout << "Sending disconnect : " << event.serverEntityId.id() << std::endl;

    sendEventToServer(event);

    mStatus = Disconnected;
}
void NetworkClientSystem::sendConnection()
{
    NetworkEvent event;
    event.type = NetworkEvent::Type::Connect;
    event.clientUuid = Tools::uuidFromTimestamp();
    mUuid = event.clientUuid;

    std::cout << "Sending connection with uuid : " << mUuid << std::endl;

    sendEventToServer(event);

    mStatus = Pending;
}
