#ifndef ZR_MOVINGSYSTEM_HPP
#define ZR_MOVINGSYSTEM_HPP
#include "../Common.hpp"
#include "../components/Events.hpp"
#include "../components/Movable.hpp"
#include "../components/Renderable.hpp"
/**
 * @brief System permettant de déplacer les entités mobiles.
 *
 */
class MovingSystem : public entityx::System<MovingSystem>, public entityx::Receiver<MovingSystem>
{
  public:
    MovingSystem()
    {
    }
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);
    void receive(const CollisionEvent &event);

  private:
    void handleKeyBoardEvents(Movable &movable);
    void handleCollisionEvents(entityx::Entity &entity, Movable &movable, Renderable &renderable);
    std::map<entityx::Entity::Id, CollisionEvent> mCollisions;
};
#endif