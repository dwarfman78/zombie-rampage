#ifndef ZR_COLLISIONSYSTEM_HPP
#define ZR_COLLISIONSYSTEM_HPP
#include "../Common.hpp"
#include "../Tools.hpp"
#include "../components/Events.hpp"
#include "../components/Movable.hpp"
#include "../components/Renderable.hpp"
#include "../components/TileMap.hpp"
class CollisionSystem : public entityx::System<CollisionSystem>, public entityx::Receiver<CollisionSystem>
{
  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);
    void receive(const CollisionEvent &event);
    void receive(const MouseEvent &event);

  private:
  void sendCollisionEvent();
  MouseEvent mMouseEvent;
};

#endif