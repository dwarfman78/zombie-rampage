#ifndef ZR_NETWORKABLE
#define ZR_NETWORKABLE
#include "../Common.hpp"
#include "../Usings.hpp"
#include "../components/Movable.hpp"
#include "../components/Renderable.hpp"

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
    bool pushState(const ::State &state)
    {
        bool pushed = true;
        if (stateBuffer.size() < BUFFER_SIZE)
        {
            stateBuffer.push(state);
        }
        else
        {
            std::cout << "Buffer overflow !" << std::endl;
            pushed = false;
        }
        return pushed;
    }

    entityx::Entity::Id localId;
    entityx::Entity::Id distantId;
    bool interpolating{false};
    bool interpolated{false};
    unsigned int clientServerDelay{0};
    bool isDesync{false};
    std::queue<::State> stateBuffer;
    constexpr static size_t BUFFER_SIZE = TICK_RATE * 5;
    bool needSendState{false};
};
#endif
