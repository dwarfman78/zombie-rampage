#ifndef ZR_KeyBoardManagingSystem_HPP
#define ZR_KeyBoardManagingSystem_hpp
#include "../Common.hpp"
#include "../components/Actionable.hpp"
#include "../components/Events.hpp"
#include "../components/Playable.hpp"
class KeyBoardManagingSystem : public entityx::System<KeyBoardManagingSystem>,
                               public entityx::Receiver<KeyBoardManagingSystem>
{

  public:
    KeyBoardManagingSystem(sf::RenderWindow &pWindow) : mWindow(pWindow)
    {
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);
    void receive(const sf::Event &event);

  private:
    sf::RenderWindow &mWindow;
};
#endif