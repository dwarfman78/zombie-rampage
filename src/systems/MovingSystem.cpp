#include "../../include/systems/MovingSystem.hpp"
void MovingSystem::configure(entityx::EventManager &event_manager)
{
    event_manager.subscribe<CollisionEvent>(*this);
    event_manager.subscribe<NetworkEvent>(*this);
}
void MovingSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<Movable, Renderable, Actionable, Networkable>([&](entityx::Entity entity, Movable &movable,
                                                              Renderable &renderable, Actionable &actionable,
                                                              Networkable &networkable) {
        std::string iama;
        if (mIsServer)
        {
            iama = "server";
        }
        else
        {
            iama = "client";
        }
        float deltaTimeFinal = static_cast<float>(dt);

        if (entity.has_component<Playable>())
        {
            handleMovingActions(actionable, movable);
        }

        handleCollisionEvents(entity, movable, renderable);
//        if (movable.mAcceleration.lengthSq() != 0)
//        {
//            std::cout << "( " << iama << " )"
//                      << "DeltaTime : " << deltaTimeFinal << " mPos : " << renderable.mPos.x << "," << renderable.mPos.y
//                      << "\n mAcc : " << movable.mAcceleration.x << "," << movable.mAcceleration.y << std::endl;
//        }
        renderable.mPos += movable.mAcceleration * (deltaTimeFinal / 1000.f);
        renderable.mRect = {renderable.mPos, {32, 32}};
    });
}
void MovingSystem::handleCollisionEvents(entityx::Entity &entity, Movable &movable, Renderable &renderable)
{
    auto collision = mCollisions.find(entity.id());
    if (collision != mCollisions.end())
    {
        auto collisionEvent = collision->second;

        sf::FloatRect intersection = collisionEvent.mIntersect;

        if (intersection.width < intersection.height)
        {
            movable.mAcceleration.x = 0;

            if (collisionEvent.mEntityAcceleration.x > 0.f)
            {
                renderable.mPos.x -= collisionEvent.mIntersect.width;
            }
            else
            {
                renderable.mPos.x += collisionEvent.mIntersect.width;
            }
        }
        else
        {
            movable.mAcceleration.y = 0;

            if (collisionEvent.mEntityAcceleration.y > 0.f)
            {
                renderable.mPos.y -= collisionEvent.mIntersect.height;
            }
            else
            {
                renderable.mPos.y += collisionEvent.mIntersect.height;
            }
        }

        mCollisions.erase(entity.id());
    }
}
void MovingSystem::handleMovingActions(Actionable &actionable, Movable &movable)
{
    if (actionable.actions[Actionable::Action::RUNLEFT])
    {
        if (movable.mAcceleration.x > 0)
        {
            movable.mAcceleration.x = 0;
        }
        if (movable.mAcceleration.x > -100)
        {
            movable.mAcceleration.x -= 2;
        }
        else
        {
            movable.mAcceleration.x = -100;
        }
    }
    else if (actionable.actions[Actionable::Action::RUNRIGHT])
    {
        if (movable.mAcceleration.x < 0)
        {
            movable.mAcceleration.x = 0;
        }

        if (movable.mAcceleration.x < 100)
        {
            movable.mAcceleration.x += 2;
        }
        else
        {
            movable.mAcceleration.x = 100;
        }
    }
    else
    {
        if (movable.mAcceleration.x > 0)
        {
            if (movable.mAcceleration.x <= 2)
            {
                movable.mAcceleration.x = 0;
            }
            else
            {
                movable.mAcceleration.x -= 3;
            }
        }
        if (movable.mAcceleration.x < 0)
        {
            if (movable.mAcceleration.x >= 2)
            {
                movable.mAcceleration.x = 0;
            }
            else
            {
                movable.mAcceleration.x += 3;
            }
        }
    }

    if (actionable.actions[Actionable::Action::RUNUP])
    {
        if (movable.mAcceleration.y > 0)
        {
            movable.mAcceleration.y = 0;
        }
        if (movable.mAcceleration.y > -100)
        {
            movable.mAcceleration.y -= 2;
        }
        else
        {
            movable.mAcceleration.y = -100;
        }
    }
    else if (actionable.actions[Actionable::Action::RUNDOWN])
    {
        if (movable.mAcceleration.y < 0)
        {
            movable.mAcceleration.y = 0;
        }
        if (movable.mAcceleration.y < 100)
        {
            movable.mAcceleration.y += 2;
        }
        else
        {
            movable.mAcceleration.y = 100;
        }
    }
    else
    {
        if (movable.mAcceleration.y > 0)
        {
            if (movable.mAcceleration.y <= 2)
            {
                movable.mAcceleration.y = 0;
            }
            else
            {
                movable.mAcceleration.y -= 3;
            }
        }

        if (movable.mAcceleration.y < 0)
        {
            if (movable.mAcceleration.y >= -2)
            {
                movable.mAcceleration.y = 0;
            }
            else
            {
                movable.mAcceleration.y += 3;
            }
        }
    }
}
void MovingSystem::receive(const CollisionEvent &event)
{
    CollisionEvent ce(event);

    mCollisions[event.mEntityId] = ce;
}
void MovingSystem::receive(const NetworkEvent &event)
{
}
