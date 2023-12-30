#ifndef ZR_NETWORKEVENT_HPP
#define ZR_NETWORKEVENT_HPP
#include "../Common.hpp"
#include "../Tools.hpp"
#include "../components/Movable.hpp"
#include "../components/Networkable.hpp"
#include "../components/Renderable.hpp"
struct NetworkEvent
{
    enum Type
    {
        Connect,
        Disconnect,
        WorldState,
        EntityCreation,
        EntitySuppression,
        KeyPressed,
        KeyReleased,
        PlayerAction,
        Message,
        Ping
    };
    NetworkEvent() : currentTimestamp(Tools::currentTimestamp())
    {
    }
    /**
     * @brief Prepare a packet to send from an Event.
     *
     * @param packet packet to send.
     * @param event Event to send.
     * @return sf::Packet& packet to send.
     */
    friend sf::Packet &operator<<(sf::Packet &packet, const NetworkEvent &event)
    {
        switch (event.type)
        {
        case Connect:
            return sendCommon(packet, event) << event.clientUuid;
            break;
        case Disconnect:
            return sendCommon(packet, event) << event.serverEntityId.id();
            break;
        case Message:
            return sendCommon(packet, event) << event.message;
            break;
        case WorldState:
            return sendWorldState(packet, event);
            break;
        case EntityCreation:
            return sendCommon(packet, event)
                   << event.serverEntityId.id() << event.clientUuid << event.entityPosition.x << event.entityPosition.y;
            break;
        case EntitySuppression:
            return sendCommon(packet, event) << event.serverEntityId.id();
            break;
        case KeyPressed:
            return sendCommon(packet, event) << event.serverEntityId.id() << static_cast<int32_t>(event.keyCode);
        case KeyReleased:
            return sendCommon(packet, event) << event.serverEntityId.id() << static_cast<int32_t>(event.keyCode);
            break;
        case PlayerAction:
            return sendCommon(packet, event)
                   << event.serverEntityId.id() << Tools::actionsToBitset(event.actions).to_string();
            break;
        default:
            return sendCommon(packet, event);
        }
    }

    /**
     * @brief Receive a packet and transform to event.
     *
     * @param packet packet received
     * @param event event to populate
     * @return sf::Packet& packet received.
     */
    friend sf::Packet &operator>>(sf::Packet &packet, NetworkEvent &event)
    {
        int32_t receive;
        packet >> receive;
        event.type = static_cast<NetworkEvent::Type>(receive);

        int64_t currentTimestamp;
        packet >> currentTimestamp;
        event.currentTimestamp = currentTimestamp;

        switch (event.type)
        {
        case Connect:
            packet >> event.clientUuid;
        case Message:
            packet >> event.message;
            break;
        case KeyPressed: {
            receiveEntityId(packet, event);
            int32_t receiveKeyCode;
            packet >> receiveKeyCode;
            event.keyCode = static_cast<sf::Keyboard::Key>(receiveKeyCode);
        }
        break;
        case KeyReleased: {
            receiveEntityId(packet, event);
            int32_t receiveKeyCode;
            packet >> receiveKeyCode;
            event.keyCode = static_cast<sf::Keyboard::Key>(receiveKeyCode);
        }
        break;
        case PlayerAction: {
            receiveEntityId(packet, event);
            receiveEntityActions(packet, event);
        }
        break;
        case WorldState: {

            receiveWorldState(packet, event);
        }
        break;
        case EntityCreation: {

            receiveEntityId(packet, event);

            packet >> event.clientUuid >> event.entityPosition.x >> event.entityPosition.y;
        }
        break;
        case EntitySuppression: {
            receiveEntityId(packet, event);
        }
        break;
        case Disconnect: {
            receiveEntityId(packet, event);
        }
        break;
        default:;
        }

        return packet;
    }

    static void receiveEntityId(sf::Packet &packet, NetworkEvent &event)
    {
        int64_t id;
        packet >> id;
        entityx::Entity::Id receivedId(id);
        event.serverEntityId = receivedId;
    }

    static void receiveEntityActions(sf::Packet &packet, NetworkEvent &event)
    {
        std::string bitstring;
        packet >> bitstring;
        std::bitset<4> bitset(bitstring);

        for (unsigned int i = 0; i < bitset.size(); ++i)
        {
            event.actions[static_cast<Actionable::Action>(i)] = bitset[i];
        }
    }

    static sf::Packet &sendCommon(sf::Packet &packet, const NetworkEvent &event)
    {
        packet << static_cast<int32_t>(event.type) << event.currentTimestamp;

        return packet;
    }

    static sf::Packet &sendWorldState(sf::Packet &packet, const NetworkEvent &event)
    {
        sendCommon(packet, event) << event.serverTick << event.worldState.size();

        for (auto it = event.worldState.begin(); it != event.worldState.end(); ++it)
        {
            auto tupleWorld = it->second;
            packet << (it->first).id();
            packet << std::get<0>(tupleWorld).mPos.x << std::get<0>(tupleWorld).mPos.y
                   << std::get<1>(tupleWorld).mAcceleration.x << std::get<1>(tupleWorld).mAcceleration.y;
        }
        return packet;
    }

    static void receiveWorldState(sf::Packet &packet, NetworkEvent &event)
    {
        size_t entitySize;
        unsigned int serverTick;

        packet >> serverTick;

        event.serverTick = serverTick;

        packet >> entitySize;

        for (auto i = 0; i < entitySize; ++i)
        {
            int64_t id;
            packet >> id;
            entityx::Entity::Id receivedId(id);

            sf::Vector2f entityPos, entityAcc;

            packet >> entityPos.x >> entityPos.y;
            packet >> entityAcc.x >> entityAcc.y;

            Renderable renderable(entityPos);

            Movable movable(entityAcc);

            Networkable networkable;
            networkable.distantId = receivedId;

            event.worldState[receivedId] = {renderable, movable, networkable};
        }
    }

    std::string message;
    std::string clientUuid;
    sf::Vector2f entityPosition;
    std::map<entityx::Entity::Id, std::tuple<Renderable, Movable, Networkable>> worldState;
    entityx::Entity::Id serverEntityId;
    entityx::Entity::Id clientEntityId;
    sf::Keyboard::Key keyCode;
    std::map<Actionable::Action, bool> actions;
    Type type;
    int64_t currentTimestamp{0};
    int64_t latency{0};
    unsigned int serverTick{0};
};
#endif
