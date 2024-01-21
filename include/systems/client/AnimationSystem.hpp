#ifndef ZR_ANIMATIONSYSTEM_HPP
#define ZR_ANIMATIONSYSTEM_HPP
#include "../../Common.hpp"
#include "../../components/Animable.hpp"
/**
  * Classe permettant de gérer les animations des entités.
**/
class AnimationSystem : public entityx::System<AnimationSystem>
{
  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
};
#endif
