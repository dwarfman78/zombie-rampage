#ifndef ZR_TOOLS_HPP
#define ZR_TOOLS_HPP
#include "Common.hpp"
class Tools
{
  public:
    static const int TILE_SIZE = 32;
    static bool segmentIntersects(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
    {
        float tn = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
        float td = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        float un = (x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2);
        float ud = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        return (tn > 0.f && td > 0.f && tn <= td) ||
               (tn < 0.f && td < 0.f && tn >= td) && (un > 0.f && ud > 0.f && un <= ud) ||
               (un < 0.f && ud < 0.f && un >= ud);
    }
    static void vectorToString(const std::string &pName, const sf::Vector2f &pVector)
    {
        std::cout << pName << " : (" << pVector.x << "," << pVector.y << ")" << std::endl;
    }
    static int globalToTile(float global)
    {
        return (int)(global / Tools::TILE_SIZE);
    }
    static float tileToGlobal(int tile)
    {
        return tile * Tools::TILE_SIZE;
    }
    static float globalToTileToGlobal(float global)
    {
        return tileToGlobal(globalToTile(global));
    }
};

#endif