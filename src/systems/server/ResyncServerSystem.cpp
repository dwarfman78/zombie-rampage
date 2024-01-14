#include "../include/systems/server/ResyncServerSystem.hpp"
void ResyncServerSystem::configure(entityx::EventManager &event_manager)
{
}
void ResyncServerSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<Movable, Renderable, Actionable, Networkable>([&](entityx::Entity entity, Movable &movable,
                                                              Renderable &renderable, Actionable &actionable,
                                                              Networkable &networkable) {
        // networkable.isDesync tells if the local(server) entity is late from the distant entity.
        // if the distant entity is desync we have to :
        // check if it is a stop running situation or a start running situation
        // if it's a start running situation the entity position is too late. We have to
        // directly go to the client position (without the client communicating its position)
        // we have to calculate the acceleration
        // if it's a stop running situation the entity position is too far.
        //
        if (networkable.isDesync)
        {
            networkable.isDesync = false;
            unsigned int tickLate = networkable.clientServerDelay / WINDOW_SIZE_MILLI;
            std::cout << tickLate << std::endl;
        }
    });
}
