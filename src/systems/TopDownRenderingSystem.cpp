#include "../../include/systems/TopDownRenderingSystem.hpp"
void TopDownRenderingSystem::configure(entityx::EventManager &event_manager)
{
    // Chargement des textures
    if (mTileTexture.loadFromFile("assets/tile.png"))
    {
        std::cout << "   assets/tile.png loaded" << std::endl;

        mTileState.texture = &mTileTexture;
    }

    if (mPlayerTexture.loadFromFile("assets/player.png"))
    {
        std::cout << "   assets/player.png loaded" << std::endl;

        mPlayerState.texture = &mPlayerTexture;
    }
}
void TopDownRenderingSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    // Affichage de la tileMap de fond.
    drawBackground(es);

    // Affichage du joueur.
    drawPlayer(es);
}
void TopDownRenderingSystem::drawPlayer(entityx::EntityManager &es)
{
    // Pour chaque Joueur...
    es.each<Playable, Renderable, Movable>(
        [&](entityx::Entity entity, Playable &playable, Renderable &renderable, Movable &movable) {
            // Constitution du vecteur de points du joueur et affichage.
            sf::VertexArray quad(sf::TriangleStrip, 4);
            quad[0].position = sf::Vector2f(renderable.mPos.x, renderable.mPos.y);
            quad[1].position = sf::Vector2f(renderable.mPos.x, renderable.mPos.y + mTileSize);
            quad[2].position = sf::Vector2f(renderable.mPos.x + mTileSize, renderable.mPos.y);
            quad[3].position = sf::Vector2f(renderable.mPos.x + mTileSize, renderable.mPos.y + mTileSize);

            quad[0].texCoords = sf::Vector2f(0, 0);
            quad[1].texCoords = sf::Vector2f(0, mTileSize);
            quad[2].texCoords = sf::Vector2f(mTileSize, 0);
            quad[3].texCoords = sf::Vector2f(mTileSize, mTileSize);

            // On dessine le joueur.
            mTarget.draw(quad, mPlayerState);

        });
}
void TopDownRenderingSystem::drawBackground(entityx::EntityManager &es)
{
    auto viewPort = mTarget.getViewport(mTarget.getDefaultView());

    es.each<TileMap>([&](entityx::Entity entity, TileMap &map) {
        // On détermine le nb de Tile à afficher par rapport à la map. (deux std::vector imbriqués)
        unsigned int nbTilesX = map.tileMap[0].size();
        unsigned int nbTilesY = map.tileMap.size();

        // La taille totale en multipliant lignes * colonnes
        unsigned int nbTiles = nbTilesX * nbTilesY;

        // La taille totale * 4 car il y a 4 points par tile.
        sf::VertexArray quad(sf::TriangleStrip, nbTiles * 4);

        // Pour constituer l'ensemble de points on prend chaque ligne
        // d'abord de gauche à droite puis en bout de ligne on passe à la ligne suivante
        // que l'on remplit en sens inverse : de droite à gauche.

        // Index du point courant, incrémenté de 4 en 4 à chaque tile.
        int quadIndex = 0;

        // Index dans la ligne.
        int xStart = 0;

        // Sens, true = de gauche à droite, false = de droite à gauche.
        bool sens = true;

        // Parcours des lignes
        for (int y = 0; y < nbTilesY; y++)
        {
            // Parcours des colonnes en fonction du sens.
            // De gauche à droite x va de 0 à nbTilesX -1
            // De droite à gauche x va nbTilesX -1 à 0
            for (int x = xStart; sens ? x < nbTilesX : x >= 0; sens ? x++ : x--)
            {
                // On converti les coordonnées de la tile en coordonnées "monde", (ici appelé pixel)
                // C'est l'index de la tile x la taille de la tile.
                unsigned int xToPixel = x * mTileSize;
                unsigned int yToPixel = y * mTileSize;

                if (sens)
                {
                    // Définition des points dans le sens gauche -> droite.
                    // On commence par le point en haut à gauche, puis bas gauche, puis haut droite et
                    // enfin on termine par bas droite.
                    quad[quadIndex].position = sf::Vector2f(xToPixel, yToPixel);
                    quad[quadIndex + 1].position = sf::Vector2f(xToPixel, yToPixel + mTileSize);
                    quad[quadIndex + 2].position = sf::Vector2f(xToPixel + mTileSize, yToPixel);
                    quad[quadIndex + 3].position = sf::Vector2f(xToPixel + mTileSize, yToPixel + mTileSize);
                }
                else
                {
                    // Définition des points en sens inverse de la droite vers la gauche.
                    // On commence en haut à droite, etc..
                    quad[quadIndex].position = sf::Vector2f(xToPixel + mTileSize, yToPixel + mTileSize);
                    quad[quadIndex + 1].position = sf::Vector2f(xToPixel + mTileSize, yToPixel);
                    quad[quadIndex + 2].position = sf::Vector2f(xToPixel, yToPixel + mTileSize);
                    quad[quadIndex + 3].position = sf::Vector2f(xToPixel, yToPixel);
                }

                if (map.tileMap[y][x] == 0)
                {
                    // Pose des textures, si c'est 0 on pose une texture du sol.
                    quad[quadIndex].texCoords = sf::Vector2f(0, 0);
                    quad[quadIndex + 1].texCoords = sf::Vector2f(0, mTileSize);
                    quad[quadIndex + 2].texCoords = sf::Vector2f(mTileSize, 0);
                    quad[quadIndex + 3].texCoords = sf::Vector2f(mTileSize, mTileSize);
                }
                else
                {
                    // Sinon on pose une texture de mur.
                    quad[quadIndex].texCoords = sf::Vector2f(0, mTileSize);
                    quad[quadIndex + 1].texCoords = sf::Vector2f(0, mTileSize * 2);
                    quad[quadIndex + 2].texCoords = sf::Vector2f(mTileSize, mTileSize);
                    quad[quadIndex + 3].texCoords = sf::Vector2f(mTileSize, mTileSize * 2);
                }

                // On incrémente l'index de pixel de 4 pour la tile suivante.
                quadIndex += 4;
            }

            // Initialisation du xStart pour la prochaine ligne.
            if (sens)
            {
                xStart = nbTilesX - 1;
            }
            else
            {
                xStart = 0;
            }
            // Inversion du sens.
            sens = !sens;
        }

        // On affiche la tileMap en une fois.
        mTarget.draw(quad, mTileState);
    });
}