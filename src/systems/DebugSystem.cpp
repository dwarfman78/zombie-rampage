#include "../../include/systems/DebugSystem.hpp"

void DebugSystem::configure(entityx::EventManager &event_manager)
{
    event_manager.subscribe<CollisionEvent>(*this);
    event_manager.subscribe<MouseEvent>(*this);
    event_manager.subscribe<NetworkEvent>(*this);
    event_manager.subscribe<InterpolationEvent>(*this);

    if (!mDebugFont.loadFromMemory(fontArmyData, fontArmyDataSize))
    {
        std::cout << "Cannot load font from memory !" << std::endl;
    }
}
void DebugSystem::drawPanel(entityx::TimeDelta dt)
{
    Tools::drawText(std::to_string((int)dt), 20, {0.f, 0.f}, mTarget);
}
void DebugSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<Actionable, Renderable, Movable>(
        [&](entityx::Entity entity, Actionable &actionable, Renderable &renderable, Movable &movable) {
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
            // drawRectOutlined(topLeftRect, sf::Color::White, Tools::TILE_SIZE, Tools::TILE_SIZE);
            // drawRectOutlined(topRightRect, sf::Color::White, Tools::TILE_SIZE, Tools::TILE_SIZE);
            // drawRectOutlined(bottomLeftRect, sf::Color::White, Tools::TILE_SIZE, Tools::TILE_SIZE);
            // drawRectOutlined(bottomRightRect, sf::Color::White, Tools::TILE_SIZE, Tools::TILE_SIZE);

            drawVector(movable.mAcceleration, renderable.mPos, sf::Color::Blue);

            if (ceOccured)
            {
                drawRectOutlined(mCe.mIntersect, sf::Color::Red, mCe.mIntersect.height, mCe.mIntersect.width);
            }

            debugWorldState(mLastNe, es);
            drawPanel(dt);
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
void DebugSystem::receive(const NetworkEvent &event)
{
    switch (event.type)
    {
    case NetworkEvent::Type::EntityCreation:

        break;
    case NetworkEvent::Type::EntitySuppression:

        break;
    case NetworkEvent::Type::WorldState:
        mLastNe = event;

        mWorldStateBuffer.push(event.worldState);

    default:;
        break;
    }
}
void DebugSystem::receive(const InterpolationEvent &event)
{
    mLastWorldState = event.currentWorldState;
}
void DebugSystem::debugWorldState(const NetworkEvent &event, entityx::EntityManager &es)
{

    es.each<Networkable>([&](entityx::Entity entity, Networkable &networkable) {
        if (networkable.stateBuffer.size() > 0)
        {

            bool firstBuff = true;
            auto copyBuff = networkable.stateBuffer;
            while (copyBuff.size() > 0)
            {
            sf::Color bufCol = firstBuff ? sf::Color::Red : sf::Color::Yellow;
                firstBuff = false;
                auto &state = copyBuff.front();
                copyBuff.pop();
                auto &[serverTick, serverRenderable, serverMovable, serverNetworkable] = state;
                sf::FloatRect rect = {serverRenderable.mPos, {Tools::TILE_SIZE, Tools::TILE_SIZE}};
                drawRectOutlined(rect, bufCol, Tools::TILE_SIZE, Tools::TILE_SIZE);
                drawVector(serverMovable.mAcceleration, serverRenderable.mPos, sf::Color::Green);
            }
        }
    });
}
