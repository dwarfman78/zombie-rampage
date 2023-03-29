#ifndef ZR_NETWORKCLIENTSYSTEM_HPP
#define ZR_NETWORKCLIENTSYSTEM_HPP
#include "../Common.hpp"
#include "../Tools.hpp"
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
    void sendConnection();
    void sendDisconnection(entityx::Entity::Id id);
    void sendHeartBeat();

    sf::UdpSocket mSocket;
    float mCumulDeltaT{0.f};
    float mLastRun{0.f};
    Status mStatus{Disconnected};
    bool mDying{false};
    entityx::Entity::Id mId;
    std::string mUuid;
    std::map<entityx::Entity::Id, entityx::Entity::Id> mDistantToLocalEntitiesIds;
    std::map<entityx::Entity::Id, entityx::Entity::Id> mLocalToDistantEntitiesIds;
};
#endif