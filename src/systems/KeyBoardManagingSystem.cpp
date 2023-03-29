#include "../../include/systems/KeyBoardManagingSystem.hpp"
void KeyBoardManagingSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<sf::Event>(*this);
}
void KeyBoardManagingSystem::update(entityx::EntityManager &es,
                                    entityx::EventManager &events,
                                    entityx::TimeDelta dt) {}
void KeyBoardManagingSystem::receive(const sf::Event &event) {
  // Add here mapping between keyboard key pressed an user actions
  // will do it later caus' i'm lazy
  // will regret it when refactoring

  if (event.type == sf::Event::Closed)
    mWindow.close();

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      mWindow.close();
    }
  }
}