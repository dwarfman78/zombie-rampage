#ifndef ZR_NETWORKEVENT_HPP
#define ZR_NETWORKEVENT_HPP
#include "../Common.hpp"
struct NetworkEvent
{
    enum Type
    {
        Connect,
        Disconnect,
        EntityPosition,
        EntityCreation,
        EntitySuppression,
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

    std::string message;
    std::string clientUuid;
    sf::Vector2f entityPosition;
    entityx::Entity::Id entityId;
    Type type;
};
#endif