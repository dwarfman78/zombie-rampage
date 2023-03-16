#ifndef ZR_RENDERABLE_HPP
#define ZR_RENDERABLE_HPP

#include "../Common.hpp"

struct Renderable
{
    Renderable(sf::Vector2f pPos) : mPos(pPos){}
    sf::Vector2f mPos;
    sf::FloatRect mRect;
};

#endif