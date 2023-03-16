#ifndef ZR_DEBUGSYSTEM
#define ZR_DEBUGSYSTEM
#include "../Tools.hpp"
#include "../components/Events.hpp"
#include "../components/Movable.hpp"
#include "../components/Playable.hpp"
#include "../components/Renderable.hpp"
#include "../components/TileMap.hpp"
#include "RenderingSystem.hpp"
class DebugSystem : public RenderingSystem<DebugSystem>, public entityx::Receiver<DebugSystem>
{
  public:
    /* Permet de réutiliser le constructeur de la classe de base */
    using RenderingSystem::RenderingSystem;

    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);
    void receive(const CollisionEvent &event);
    void receive(const MouseEvent &event);
    void drawRectOutlined(const sf::FloatRect &rect, sf::Color color, float height, float width);
    void drawVector(const sf::Vector2f &vector, const sf::Vector2f &where, sf::Color color);

  private:
    bool ceOccured = false;
    CollisionEvent mCe;
    MouseEvent mMouseEvent;
};
#endif