#pragma once
#include "../src/Math/vector2.h"

class PathNode
{
private:
    bool is_walkable;
    Vector2 world_position;
    int map_x;
    int map_y;

    int g_cost;
    int h_cost;

    PathNode* parent;
    std::vector<PathNode*> neighbours;

public:
    PathNode(const bool is_walkable, const float world_x, const float world_y, const int map_x, const int map_y) :
        is_walkable(is_walkable), world_position(world_x, world_y), map_x(map_x), map_y(map_y),
        g_cost(0), h_cost(0), parent(nullptr) {}

    void AddNeighbours(PathNode* node)
    {
        neighbours.emplace_back(node);
    }

    std::vector<PathNode*>& GetNeighbours() { return neighbours; }

    bool GetIsWalkable() const { return is_walkable; }
    const Vector2& GetWorldPosition() const { return world_position; } 
    int GetHCost() const { return g_cost + h_cost; }
    int GetMapX() const { return map_x; }
    int GetMapY() const { return map_y; }
};
