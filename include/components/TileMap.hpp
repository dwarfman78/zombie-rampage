#include "../Common.hpp"

#ifndef ZR_TILEMAP_HPP
#define ZR_TILEMAP_HPP

#include "../Tools.hpp"
struct TileMap
{
    // std::vector<std::vector<unsigned int>> tileMap = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //                                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    //                                                   {1, 0, 0, 0, 0, 1, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //                                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1,
    //                                                   1}};
    std::vector<std::vector<unsigned int>> tileMap;
    TileMap()
    {
        /// Créer un générateur de nombres aléatoires (Mersenne Twister)
        std::random_device rd;
        std::mt19937 generator(rd());

        // Créer une distribution uniforme entre 0 et 1
        std::uniform_int_distribution<int> distribution(0, 1);

        for (int i{0}; i < 1080 / Tools::TILE_SIZE; ++i)
        {
            std::vector<unsigned int> row;
            for (int j{0}; j < 1920 / Tools::TILE_SIZE; ++j)
            {
                if (i == 0 || i == 1080 / Tools::TILE_SIZE - 1 || j == 1920 / Tools::TILE_SIZE - 1 || j == 0)
                {
                    row.push_back(1);
                }
                else
                {
                    row.push_back(0/*distribution(generator)*/);
                }
            }
            tileMap.push_back(row);
        }
    }
};

#endif
