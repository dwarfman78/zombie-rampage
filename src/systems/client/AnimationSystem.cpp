#include "../../../include/systems/client/AnimationSystem.hpp"
void AnimationSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<Animable>([&](entityx::Entity entity, Animable &animable) {
        auto curAnimOpt = animable.getCurrentAnimation();

        if (curAnimOpt.has_value())
        {
            auto curAnim = curAnimOpt.value();

            curAnim->addDisplayTime((float)dt);

            if (curAnim->getDisplayTime() >= curAnim->getFrameDuration())
            {
                curAnim->nextFrame();
            }
        }
    });
}
