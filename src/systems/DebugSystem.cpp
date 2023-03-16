#include "../../include/systems/DebugSystem.hpp"

void DebugSystem::configure(entityx::EventManager &event_manager)
{
    event_manager.subscribe<CollisionEvent>(*this);
    event_manager.subscribe<MouseEvent>(*this);
}
void DebugSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<Playable, Renderable, Movable>(
        [&](entityx::Entity entity, Playable &playable, Renderable &renderable, Movable &movable) {
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

            drawRectOutlined(playerRect, sf::Color::White, Tools::TILE_SIZE, Tools::TILE_SIZE);
            drawRectOutlined(topLeftRect, sf::Color::White, Tools::TILE_SIZE, Tools::TILE_SIZE);
            drawRectOutlined(topRightRect, sf::Color::White, Tools::TILE_SIZE, Tools::TILE_SIZE);
            drawRectOutlined(bottomLeftRect, sf::Color::White, Tools::TILE_SIZE, Tools::TILE_SIZE);
            drawRectOutlined(bottomRightRect, sf::Color::White, Tools::TILE_SIZE, Tools::TILE_SIZE);

            drawVector(movable.mAcceleration, renderable.mPos, sf::Color::Blue);

            if (ceOccured)
            {
                drawRectOutlined(mCe.mIntersect, sf::Color::Red, mCe.mIntersect.height, mCe.mIntersect.width);
            }
        }

    );
}
void DebugSystem::drawRectOutlined(const sf::FloatRect &rect, sf::Color color, float height, float width)
{
    sf::VertexArray rectStrip(sf::LineStrip, 5);

    rectStrip[0].position = {rect.left, rect.top};
    rectStrip[1].position = {rect.left + width, rect.top};
    rectStrip[2].position = {rect.left + width, rect.top + height};
    rectStrip[3].position = {rect.left, rect.top + height};
    rectStrip[4].position = rectStrip[0].position;

    rectStrip[0].color = color;
    rectStrip[1].color = color;
    rectStrip[2].color = color;
    rectStrip[3].color = color;
    rectStrip[4].color = color;

    mTarget.draw(rectStrip);
}
void DebugSystem::drawVector(const sf::Vector2f &vector, const sf::Vector2f &where, sf::Color color)
{
    sf::VertexArray line(sf::Lines, 2);

    line[0].position = where;
    line[1].position = where + vector;

    line[0].color = color;
    line[1].color = color;

    mTarget.draw(line);
}
void DebugSystem::receive(const CollisionEvent &event)
{
    CollisionEvent ce(event);

    mCe = ce;

    ceOccured = true;
}
void DebugSystem::receive(const MouseEvent &event)
{
    MouseEvent me(event);
    mMouseEvent = me;
}