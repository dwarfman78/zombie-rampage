#include "../../include/systems/MouseManagingSystem.hpp"

void MouseManagingSystem::configure(entityx::EventManager &event_manager)
{
    // event_manager.subscribe<sf::Event>(*this);
}
void MouseManagingSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    MouseEvent me;

    me.mPos = sf::Mouse::getPosition(mWindow);

    events.emit(me);
}