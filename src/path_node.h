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
    int p_cost;

    PathNode* parent;
    std::vector<PathNode*> neighbours;

public:
    PathNode(const bool is_walkable, const float world_x, const float world_y, const int map_x, const int map_y) :
        is_walkable(is_walkable), world_position(world_x, world_y), map_x(map_x), map_y(map_y),
        g_cost(0), h_cost(0), p_cost(0), parent(nullptr), heap_index(0)
    {
    }

    void AddNeighbours(PathNode* node)
    {
        neighbours.emplace_back(node);
    }

    std::vector<PathNode*>& GetNeighbours() { return neighbours; }

    bool GetIsWalkable() const { return is_walkable; }
    void SetIsWalkable(bool walkable) { is_walkable = walkable; }
    
    const Vector2& GetWorldPosition() const { return world_position; }

    void SetGCost(const int cost) { g_cost = cost; }
    void SetHCost(const int cost) { h_cost = cost; }
    void SetPCost(const int cost) { p_cost = cost; }

    int GetGCost() const { return g_cost; }
    int GetHCost() const { return h_cost; }
    int GetFCost() const { return g_cost + h_cost + p_cost; }

    int GetMapX() const { return map_x; }
    int GetMapY() const { return map_y; }

    void SetParent(PathNode* parent_node) { parent = parent_node; }
    PathNode* GetParent() const { return parent; }
    
    int heap_index;
    
    int CompareTo(const PathNode& other) const
    {
        if (GetFCost() > other.GetFCost())
            return -1;

        if (GetFCost() < other.GetFCost())
            return 1;

        if (GetHCost() >= other.GetHCost())
            return -1;
        
        return 1;
    }
};
