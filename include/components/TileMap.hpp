#include "../Common.hpp"

#ifndef ZR_TILEMAP_HPP
#define ZR_TILEMAP_HPP

struct TileMap
{
    std::vector<std::vector<unsigned int>> tileMap = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                      {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                                                      {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                      {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
};

#endif