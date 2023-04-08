#ifndef ZR_NETWORKSERVERSYSTEM_HPP
#define ZR_NETWORKSERVERSYSTEM_HPP
#include "../Common.hpp"
#include "../components/Actionable.hpp"
#include "../components/Movable.hpp"
#include "../components/NetworkEvent.hpp"
#include "../components/Playable.hpp"
#include "../components/Renderable.hpp"
class NetworkServerSystem : public entityx::System<NetworkServerSystem>
{
  public:
    NetworkServerSystem()
    {
    }
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);

  private:
    void sendAllEntities(entityx::EntityManager &es, const std::tuple<std::string, sf::IpAddress, unsigned short> &to);
    void broadcastEntityCreation(entityx::Entity &e);
    void broadcastEntitySuppression(const entityx::Entity::Id &entityId);
    void broadcastEntityPositions(entityx::EntityManager &es);
    void handlePacket(entityx::EntityManager &es, sf::Packet &packet, sf::IpAddress &senderAdress,
                      unsigned short senderPort);
    void handleConnect(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event, sf::IpAddress &senderAdress,
                       unsigned short senderPort);
    void handleDisconnect(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                          sf::IpAddress &senderAdress, unsigned short senderPort);
    void handleMessage(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event, sf::IpAddress &senderAdress,
                       unsigned short senderPort);
    void handleKeyPressed(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                          sf::IpAddress &senderAdress, unsigned short senderPort);
    void handleKeyReleased(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                           sf::IpAddress &senderAdress, unsigned short senderPort);
    void handlePlayerAction(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event,
                            sf::IpAddress &senderAdress, unsigned short senderPort);
    void handleDefault(entityx::EntityManager &es, sf::Packet &packet, NetworkEvent &event, sf::IpAddress &senderAdress,
                       unsigned short senderPort);
    sf::UdpSocket mSocket;
    sf::SocketSelector mSelector;
    std::map<entityx::Entity::Id, std::tuple<std::string, sf::IpAddress, unsigned short>> mLocalEntitiesToDistantIps;
    float mCumulDeltaT{0.f};
    float mLastRun{0.f};
};
#endif