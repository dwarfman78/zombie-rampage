#ifndef ZR_MOVABLE_HPP
#define ZR_MOVABLE_HPP
#include "../Common.hpp"
struct Movable
{
    Movable(sf::Vector2f pAcceleration) : mAcceleration(pAcceleration) {}
    sf::Vector2f mAcceleration;
};
#endif