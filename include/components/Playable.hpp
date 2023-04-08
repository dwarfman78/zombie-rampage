#ifndef ZR_PLAYABLE_HPP
#define ZR_PLAYABLE_HPP
#include "../Common.hpp"
struct Playable
{
    Playable(std::string pPlayerName) : playerName(pPlayerName)
    {
    }
    std::string playerName;
    entityx::Entity::Id serverSideId;
};
#endif