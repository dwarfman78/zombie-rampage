#ifndef ZR_INTERPOLATIONEVENT
#define ZR_INTERPOLATIONEVENT
#include "../Common.hpp"
#include "Movable.hpp"
#include "Networkable.hpp"
#include "Renderable.hpp"
struct InterpolationEvent
{
    using WorldState = std::map<entityx::Entity::Id, std::tuple<Renderable, Movable, Networkable>>;

    WorldState currentWorldState;
};
#endif
