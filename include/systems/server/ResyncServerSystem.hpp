#ifndef ZR_RESYNCSERVERSYSTEM
#define ZR_RESYNCSERVERSYSTEM
#include "../../Common.hpp"

#include "../../components/Actionable.hpp"
#include "../../components/Events.hpp"
#include "../../components/Movable.hpp"
#include "../../components/NetworkEvent.hpp"
#include "../../components/Playable.hpp"
#include "../../components/Renderable.hpp"
// This system takes care of synchronizing the server's entities
// regarding the delay that occurs between the time a client sends
// actions and the moment the server starts to take them into account.
class ResyncServerSystem : public entityx::System<ResyncServerSystem>
{
  public:
    ResyncServerSystem()
    {
    }
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);
};
#endif
