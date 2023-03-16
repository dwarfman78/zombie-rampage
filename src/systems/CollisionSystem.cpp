#include "../../include/systems/CollisionSystem.hpp"
void CollisionSystem::configure(entityx::EventManager &event_manager)
{
    event_manager.subscribe<CollisionEvent>(*this);
    event_manager.subscribe<MouseEvent>(*this);
}
void CollisionSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{

    entityx::ComponentHandle<TileMap> handleTileMap;
    for (auto entity : es.entities_with_components(handleTileMap))
    {
        handleTileMap = entity.component<TileMap>();
    }

    es.each<Renderable, Movable>([&](entityx::Entity entity, Renderable &renderable, Movable &movable) {
        if (movable.mAcceleration.length() > 0.f)
        {
            sf::FloatRect topLeftRect, topRightRect, bottomLeftRect, bottomRightRect;
            sf::FloatRect playerRect = renderable.mRect;

            topLeftRect = {{Tools::globalToTileToGlobal(playerRect.left), Tools::globalToTileToGlobal(playerRect.top)},
                           {Tools::TILE_SIZE, Tools::TILE_SIZE}};

            topRightRect = {{Tools::globalToTileToGlobal(playerRect.left + Tools::TILE_SIZE),
                             Tools::globalToTileToGlobal(playerRect.top)},
                            {Tools::TILE_SIZE, Tools::TILE_SIZE}};

            bottomLeftRect = {{Tools::globalToTileToGlobal(playerRect.left),
                               Tools::globalToTileToGlobal(playerRect.top + Tools::TILE_SIZE)},
                              {Tools::TILE_SIZE, Tools::TILE_SIZE}};

            bottomRightRect = {{Tools::globalToTileToGlobal(playerRect.left + Tools::TILE_SIZE),
                                Tools::globalToTileToGlobal(playerRect.top + Tools::TILE_SIZE)},
                               {Tools::TILE_SIZE, Tools::TILE_SIZE}};

            if (handleTileMap)
            {

                int wall =
                    handleTileMap->tileMap[Tools::globalToTile(topLeftRect.top)][Tools::globalToTile(topLeftRect.left)];

                if (wall)
                {
                    auto intersection = playerRect.findIntersection(topLeftRect);
                    if (intersection.has_value())
                    {
                        CollisionEvent collisionEvent;
                        collisionEvent.mIntersect = intersection.value();
                        collisionEvent.mEntityId = entity.id();
                        collisionEvent.mEntityAcceleration = movable.mAcceleration;
                        events.emit<CollisionEvent>(collisionEvent);
                    }
                }
                wall = handleTileMap
                           ->tileMap[Tools::globalToTile(topRightRect.top)][Tools::globalToTile(topRightRect.left)];

                if (wall)
                {
                    auto intersection = playerRect.findIntersection(topRightRect);
                    if (intersection.has_value())
                    {
                        CollisionEvent collisionEvent;
                        collisionEvent.mIntersect = intersection.value();
                        collisionEvent.mEntityId = entity.id();
                        collisionEvent.mEntityAcceleration = movable.mAcceleration;
                        events.emit<CollisionEvent>(collisionEvent);
                    }
                }
                wall = handleTileMap
                           ->tileMap[Tools::globalToTile(bottomLeftRect.top)][Tools::globalToTile(bottomLeftRect.left)];

                if (wall)
                {
                    auto intersection = playerRect.findIntersection(bottomLeftRect);
                    if (intersection.has_value())
                    {
                        CollisionEvent collisionEvent;
                        collisionEvent.mIntersect = intersection.value();
                        collisionEvent.mEntityId = entity.id();
                        collisionEvent.mEntityAcceleration = movable.mAcceleration;
                        events.emit<CollisionEvent>(collisionEvent);
                    }
                }

                wall =
                    handleTileMap
                        ->tileMap[Tools::globalToTile(bottomRightRect.top)][Tools::globalToTile(bottomRightRect.left)];

                if (wall)
                {
                    auto intersection = playerRect.findIntersection(bottomRightRect);
                    if (intersection.has_value())
                    {
                        CollisionEvent collisionEvent;
                        collisionEvent.mIntersect = intersection.value();
                        collisionEvent.mEntityId = entity.id();
                        collisionEvent.mEntityAcceleration = movable.mAcceleration;
                        events.emit<CollisionEvent>(collisionEvent);
                    }
                }
            }
        }
    });
}
void CollisionSystem::receive(const CollisionEvent &event)
{
}
void CollisionSystem::receive(const MouseEvent &event)
{
    MouseEvent me(event);
    mMouseEvent = me;
}