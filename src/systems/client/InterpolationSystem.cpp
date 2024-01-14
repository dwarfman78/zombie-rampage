#include "../../../include/systems/client/InterpolationSystem.hpp"
void InterpolationSystem::configure(entityx::EventManager &event_manager)
{
}
void InterpolationSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta)
{
    //    if (entities.size() > 2)
    {
        entities.each<Renderable, Movable, Networkable>([&](entityx::Entity entity, Renderable &renderable,
                                                            Movable &movable, Networkable &networkable) {
            if (networkable.stateBuffer.size() > 0)
            {
                auto &[serverTick, stateRenderable, stateMovable, stateNetworkable] = networkable.stateBuffer.front();

                ::Vec2f vOldNewPos = stateRenderable.mPos - renderable.mPos;
                movable.mAcceleration = stateMovable.mAcceleration;

                if (vOldNewPos.lengthSq() > 144)
                {
                    renderable.mPos = stateRenderable.mPos;
                    renderable.mRect = stateRenderable.mRect;
                }
                networkable.stateBuffer.pop();
            }
        });
    }
}
