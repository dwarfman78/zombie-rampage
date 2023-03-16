#ifndef ZR_EVENTS_HPP
#define ZR_EVENTS_HPP
#include "../Common.hpp"
struct CollisionEvent
{
    CollisionEvent()
    {
    }
    CollisionEvent(const CollisionEvent &pCe)
        : mEntityId(pCe.mEntityId), mEntityPos(pCe.mEntityPos), mEntityAcceleration(pCe.mEntityAcceleration),
          mIntersect(pCe.mIntersect)
    {
    }

    friend std::ostream &operator<<(std::ostream &os, const CollisionEvent &collision)
    {
        os << "Collision event : "
           << "\n\t Entity id : " << collision.mEntityId << "\n\t acceleration : {" << collision.mEntityAcceleration.x
           << "," << collision.mEntityAcceleration.y << "}"
           << "\n\t position : {" << collision.mEntityPos.x << "," << collision.mEntityPos.y << "}"
           << "\n\t intersection : {" << collision.mIntersect.width << "," << collision.mIntersect.height << "}";

        return os;
    }

    entityx::Entity::Id mEntityId;
    sf::Vector2f mEntityPos;
    sf::Vector2f mEntityAcceleration;
    sf::FloatRect mIntersect;
};
struct MouseEvent
{
    MouseEvent()
    {
    }
    MouseEvent(const MouseEvent &pMe) : mPos(pMe.mPos)
    {
    }
    sf::Vector2i mPos;
};
struct KeyboardEvent
{
    KeyboardEvent()
    {
    }
    KeyboardEvent(const KeyboardEvent &pKe) : mPressed(pKe.mPressed)
    {
    }

    sf::Keyboard::Key mPressed;
};

#endif