#ifndef ZR_INTERPOLATIONSYSTEM
#define ZR_INTERPOLATIONSYSTEM
#include "../../Common.hpp"
#include "../../components/InterpolationEvent.hpp"
#include "../../components/NetworkEvent.hpp"
#include "../../components/Networkable.hpp"
#include "../../components/Playable.hpp"
class InterpolationSystem : public entityx::System<InterpolationSystem>, public entityx::Receiver<InterpolationSystem>
{
    using WorldState = std::map<entityx::Entity::Id, std::tuple<Renderable, Movable, Networkable>>;

  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);
    void receive(const NetworkEvent &event);

  private:
    std::queue<WorldState> mWorldStateBuffer;
    std::map<entityx::Entity::Id, entityx::Entity::Id> mDistantToLocalEntitiesIds;
    std::map<entityx::Entity::Id, entityx::Entity::Id> mLocalToDistantEntitiesIds;
    unsigned int mLastServerTick{0};
};
#endif
