#ifndef ZR_NETWORKCLIENTSYSTEM_HPP
#define ZR_NETWORKCLIENTSYSTEM_HPP
#include "../Common.hpp"
#include "../Tools.hpp"
#include "../components/Actionable.hpp"
#include "../components/Movable.hpp"
#include "../components/NetworkEvent.hpp"
#include "../components/Playable.hpp"
#include "../components/Renderable.hpp"
class NetworkClientSystem : public entityx::System<NetworkClientSystem>, public entityx::Receiver<NetworkClientSystem>
{
    enum Status
    {
        Connected,
        Pending,
        Disconnected
    };

  public:
    NetworkClientSystem()
    {
    }
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);
    void receive(const sf::Event &event);

  private:
    void handlePacket(sf::Packet &packet, entityx::EntityManager &entities, entityx::EventManager &events);
    void handleEntityCreation(entityx::EntityManager &entities, NetworkEvent &incomingEvent);
    void handleEntitySuppression(entityx::EntityManager &entities, NetworkEvent &incomingEvent);
    void handleEntityPosition(entityx::EntityManager &entities, NetworkEvent &incomingEvent);
    void sendConnection();
    void sendDisconnection(entityx::Entity::Id id);
    void sendKeyPressed(entityx::Entity::Id id, const sf::Keyboard::Key &key);
    void sendKeyReleased(entityx::Entity::Id id, const sf::Keyboard::Key &key);
    void sendHeartBeat();
    void sendActions(const std::map<Actionable::Action, bool> &actions);
    void sendEventToServer(const NetworkEvent &event);

    sf::UdpSocket mSocket;
    float mCumulDeltaT{0.f};
    float mLastRun{0.f};
    Status mStatus{Disconnected};
    bool mDying{false};
    entityx::Entity::Id mId;
    std::string mUuid;
    std::bitset<4> mLastSentActions;
    std::map<entityx::Entity::Id, entityx::Entity::Id> mDistantToLocalEntitiesIds;
    std::map<entityx::Entity::Id, entityx::Entity::Id> mLocalToDistantEntitiesIds;
    std::string mServerIp{"127.0.0.1"};
    unsigned short mServerPort{3121};
};
#endif