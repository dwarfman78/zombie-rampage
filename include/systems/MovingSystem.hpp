#ifndef ZR_MOVINGSYSTEM_HPP
#define ZR_MOVINGSYSTEM_HPP
#include "../Common.hpp"
#include "../components/Actionable.hpp"
#include "../components/Events.hpp"
#include "../components/Movable.hpp"
#include "../components/NetworkEvent.hpp"
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
    void receive(const NetworkEvent &event);

  private:
    void handleMovingActions(Actionable &actionable, Movable &movable);
    void handleCollisionEvents(entityx::Entity &entity, Movable &movable, Renderable &renderable);

    // maybe should be stored at the entity level
    std::map<entityx::Entity::Id, CollisionEvent> mCollisions;
    std::map<entityx::Entity::Id, sf::Vector2f> mNetworkPositionEvents;
};
#endif