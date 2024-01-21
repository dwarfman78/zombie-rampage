#ifndef ZR_MOUSEMANAGINGSYSTEM
#define ZR_MOUSEMANAGINGSYSTEM

#include "../../Tools.hpp"
#include "../../components/Actionable.hpp"
#include "../../components/Events.hpp"
#include "../../components/Playable.hpp"
#include "../client/RenderingSystem.hpp"
class MouseManagingSystem : public entityx::System<MouseManagingSystem>
{
  public:
    MouseManagingSystem(sf::RenderWindow &pWindow) : mWindow(pWindow)
    {
    }
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);

  private:
    sf::RenderWindow &mWindow;
};
#endif
