#ifndef ZR_RENDERINGSYSTEM_HPP
#define ZR_RENDERINGSYSTEM_HPP
#include "../Common.hpp"

/**
 * @brief System abstrait permettant d'effectuer un rendu sur cible.
 *
 * @tparam SUBSYSTEM Sous system concrêt.
 */
template <class SUBSYSTEM>
class RenderingSystem : public entityx::System<SUBSYSTEM>
{
public:
    RenderingSystem(sf::RenderTarget &pTarget) : mTarget(pTarget) {}

protected:
    sf::RenderTarget &mTarget;
};
#endif