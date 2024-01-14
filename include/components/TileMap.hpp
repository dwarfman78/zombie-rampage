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
                    row.push_back(0);
                }
            }
            tileMap.push_back(row);
        }
    }
};

#endif
