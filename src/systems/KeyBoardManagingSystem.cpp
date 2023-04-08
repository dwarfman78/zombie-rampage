#include "../../include/systems/KeyBoardManagingSystem.hpp"
void KeyBoardManagingSystem::configure(entityx::EventManager &event_manager)
{
    event_manager.subscribe<sf::Event>(*this);
}
void KeyBoardManagingSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<Playable, Actionable>([&](entityx::Entity entity, Playable &playable, Actionable &actionable) {
        actionable.actions[Actionable::Action::RUNLEFT] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);

        actionable.actions[Actionable::Action::RUNRIGHT] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

        actionable.actions[Actionable::Action::RUNDOWN] = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

        actionable.actions[Actionable::Action::RUNUP] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
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