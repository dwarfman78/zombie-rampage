#ifndef ZR_NETWORKABLE
#define ZR_NETWORKABLE
#include "../Common.hpp"
struct Networkable
{
    Networkable()
    {
    }
    Networkable(const Networkable &pNetworkable)
        : localId(pNetworkable.localId), distantId(pNetworkable.distantId), interpolated(pNetworkable.interpolated),
          interpolating(pNetworkable.interpolating), clientServerDelay(pNetworkable.clientServerDelay)
    {
    }
    Networkable(entityx::Entity::Id pLocalId, entityx::Entity::Id pDistantId, unsigned int pClientServerDelay)
        : localId(pLocalId), distantId(pDistantId), clientServerDelay(pClientServerDelay)
    {
    }
    entityx::Entity::Id localId;
    entityx::Entity::Id distantId;
    bool interpolating{false};
    bool interpolated{false};
    unsigned int clientServerDelay{0};
    bool isDesync{false};
};
#endif
