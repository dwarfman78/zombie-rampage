#include "../../include/systems/KeyBoardManagingSystem.hpp"
void KeyBoardManagingSystem::configure(entityx::EventManager &event_manager)
{
    event_manager.subscribe<sf::Event>(*this);
}
void KeyBoardManagingSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<Movable>([&](entityx::Entity entity, Movable &movable) {
       
    });
}
void KeyBoardManagingSystem::receive(const sf::Event &event)
{
    if (event.type == sf::Event::Closed)
        mWindow.close();

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            mWindow.close();
        }
    }
}