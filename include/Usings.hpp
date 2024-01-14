#ifndef ZR_USINGS_HPP
#define ZR_USINGS_HPP
#include "Common.hpp"
#include "components/Movable.hpp"
#include "components/Renderable.hpp"
class Networkable;
// Define types shortcuts
using WorldState = std::map<entityx::Entity::Id, std::tuple<Renderable, Movable, Networkable>>;
using Vec2f = sf::Vector2f;
using State = std::tuple<unsigned int, Renderable, Movable, Networkable>;
#endif
