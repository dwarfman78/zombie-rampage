#include "../../../include/systems/client/MouseManagingSystem.hpp"

void MouseManagingSystem::configure(entityx::EventManager &event_manager)
{
    // event_manager.subscribe<sf::Event>(*this);
}
void MouseManagingSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    if (mWindow.hasFocus())
    {
        MouseEvent me;

        me.mPos = sf::Mouse::getPosition(mWindow);

        events.emit(me);
        es.each<Playable, Actionable>([&](entityx::Entity entity, Playable &playable, Actionable &actionable) {
            actionable.lookAt = Tools::convertToVFloat(me.mPos);
        });
    }
}
