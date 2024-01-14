#ifndef ZR_INTERPOLATIONSYSTEM
#define ZR_INTERPOLATIONSYSTEM
#include "../../Common.hpp"
#include "../../Usings.hpp"
#include "../../components/InterpolationEvent.hpp"
#include "../../components/NetworkEvent.hpp"
#include "../../components/Networkable.hpp"
#include "../../components/Playable.hpp"
class InterpolationSystem : public entityx::System<InterpolationSystem>
{
  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);

  private:
    unsigned int mLastServerTick{0};
};
#endif
