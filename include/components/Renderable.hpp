#ifndef ZR_RENDERABLE_HPP
#define ZR_RENDERABLE_HPP

#include "../Common.hpp"

struct Renderable
{
    Renderable()
    {
    }
    Renderable(sf::Vector2f pPos) : mPos(pPos)
    {
    }
    bool operator!=(const Renderable &other) const
    {
        return (mPos != other.mPos && mRect != other.mRect);
    }
    sf::Vector2f mPos;
    sf::FloatRect mRect;
};

#endif
