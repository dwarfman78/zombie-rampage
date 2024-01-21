#include "../../../include/systems/client/AnimationSystem.hpp"
void AnimationSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    // Mise à jour des animations des différentes entités en fonction du temps passé entre chaque appel et du temps
    // d'affichage attendu de chaque image des animations.
    es.each<Animable>([&](entityx::Entity entity, Animable &animable) {
        auto curAnimOpt = animable.getCurrentAnimationList();

        if (curAnimOpt.has_value())
        {
            for (auto &curAnim : *curAnimOpt.value())
            {
                curAnim.addDisplayTime((float)dt);
                // si le temps d'affichage de l'image courante est dépassé on passe à l'image suivante.
                if (curAnim.getDisplayTime() >= curAnim.getFrameDuration())
                {
                    curAnim.nextFrame();
                }
            }
        }
    });
    // Mise à jour des animations à afficher en fonction du statut de l'entité.
    es.each<Animable, Actionable>([&](entityx::Entity entity, Animable &animable, Actionable &actionable) {
        if (actionable.isRunning())
        {
            animable.setCurrentAnimation("walking");
        }
        else
        {
            animable.setCurrentAnimation("standing");
        }
    });
}
