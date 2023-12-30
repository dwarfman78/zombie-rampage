#ifndef ZR_DEBUGSYSTEM
#define ZR_DEBUGSYSTEM
#include "../Common.hpp"
#include "../Tools.hpp"
#include "client/RenderingSystem.hpp"
#include "../components/Events.hpp"
#include "../components/InterpolationEvent.hpp"
#include "../components/Movable.hpp"
#include "../components/NetworkEvent.hpp"
#include "../components/Playable.hpp"
#include "../components/Renderable.hpp"
#include "../components/TileMap.hpp"
class DebugSystem : public RenderingSystem<DebugSystem>, public entityx::Receiver<DebugSystem>
{
    using WorldState = std::map<entityx::Entity::Id, std::tuple<Renderable, Movable, Networkable>>;

  public:
    /* Permet de réutiliser le constructeur de la classe de base */
    using RenderingSystem::RenderingSystem;

    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
    void configure(entityx::EventManager &event_manager);
    void receive(const CollisionEvent &event);
    void receive(const MouseEvent &event);
    void receive(const NetworkEvent &event);
    void receive(const InterpolationEvent &event);
    void drawRectOutlined(const sf::FloatRect &rect, sf::Color color, float height, float width);
    void drawVector(const sf::Vector2f &vector, const sf::Vector2f &where, sf::Color color);
    void drawPanel(entityx::TimeDelta dt);

  private:
    void debugWorldState(const NetworkEvent &event);
    NetworkEvent mLastNe;
    WorldState mLastWorldState;
    std::queue<WorldState> mWorldStateBuffer;
    bool ceOccured = false;
    CollisionEvent mCe;
    MouseEvent mMouseEvent;
    sf::Font mDebugFont;
};
#endif
