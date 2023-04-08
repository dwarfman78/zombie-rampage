#ifndef ZR_NETWORKEVENT_HPP
#define ZR_NETWORKEVENT_HPP
#include "../Common.hpp"
#include "../Tools.hpp"
struct NetworkEvent
{
    enum Type
    {
        Connect,
        Disconnect,
        EntityPosition,
        EntityCreation,
        EntitySuppression,
        KeyPressed,
        KeyReleased,
        PlayerAction,
        Message
    };
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
            return packet << static_cast<int32_t>(event.type) << event.clientUuid;
            break;
        case Disconnect:
            return packet << static_cast<int32_t>(event.type) << event.entityId.id();
            break;
        case Message:
            return packet << static_cast<int32_t>(event.type) << event.message;
            break;
        case EntityPosition:
            return packet << static_cast<int32_t>(event.type) << event.entityId.id() << event.entityPosition.x
                          << event.entityPosition.y;
            break;
        case EntityCreation:
            return packet << static_cast<int32_t>(event.type) << event.entityId.id() << event.clientUuid
                          << event.entityPosition.x << event.entityPosition.y;
            break;
        case EntitySuppression:
            return packet << static_cast<int32_t>(event.type) << event.entityId.id();
            break;
        case KeyPressed:
            return packet << static_cast<int32_t>(event.type) << event.entityId.id()
                          << static_cast<int32_t>(event.keyCode);
        case KeyReleased:
            return packet << static_cast<int32_t>(event.type) << event.entityId.id()
                          << static_cast<int32_t>(event.keyCode);
            break;
        case PlayerAction:
            return packet << static_cast<int32_t>(event.type) << event.entityId.id()
                          << Tools::actionsToBitset(event.actions).to_string();
            break;
        default:
            return packet << static_cast<int32_t>(event.type);
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
        case EntityPosition: {

            receiveEntityId(packet, event);

            packet >> event.entityPosition.x >> event.entityPosition.y;
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
        event.entityId = receivedId;
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

    std::string message;
    std::string clientUuid;
    sf::Vector2f entityPosition;
    entityx::Entity::Id entityId;
    sf::Keyboard::Key keyCode;
    std::map<Actionable::Action, bool> actions;
    Type type;
};
#endif