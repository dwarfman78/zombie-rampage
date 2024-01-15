#ifndef ZR_TOPDOWNRENDERINGSYSTEM_HPP
#define ZR_TOPDOWNRENDERINGSYSTEM_HPP

#include "../../../include/components/Animable.hpp"
#include "../../../include/components/Movable.hpp"
#include "../../../include/components/Playable.hpp"
#include "../../../include/components/Renderable.hpp"
#include "../../../include/components/TileMap.hpp"
#include "../../Common.hpp"
#include "../../assets/player.png.hpp"
#include "../../assets/tile.png.hpp"
#include "../client/RenderingSystem.hpp"
/**
 * @brief System de rendu en vue du dessus (top down)
 *
 */
class TopDownRenderingSystem : public RenderingSystem<TopDownRenderingSystem>
{
  public:
    /* Permet de réutiliser le constructeur de la classe de base */
    using RenderingSystem::RenderingSystem;

    /**
     * @brief configuration du system.
     *
     * @param event_manager manager.
     */
    void configure(entityx::EventManager &event_manager);

    /**
     * @brief application du system.
     *
     * @param es entity manager.
     * @param events event manager.
     * @param dt intervalle de temps entre 2 appels.
     */
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

  private:
    /**
     * @brief Affiche le fond.
     *
     * @param es entity manager.
     */
    void drawBackground(entityx::EntityManager &es);

    /**
     * @brief Affiche le joueur.
     *
     * @param es entity manager.
     */
    void drawPlayer(entityx::EntityManager &es);

    /* Taille d'une tuile */
    unsigned int mTileSize = 32;

    /* Background sprites */
    sf::Texture mTileTexture;
    sf::RenderStates mTileState;

    /* Player sprites */
    sf::Texture mPlayerTexture;
    sf::RenderStates mPlayerState;
};

#endif
