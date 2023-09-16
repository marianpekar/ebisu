#pragma once

#include "../../Math/vector2.h"
#include "component.h"

class MapCollider final : public Component
{
private:
    Vector2 position; 
    float width, height;
    int map_size, map_tile_size;
    class Map* map;
    bool HasCollisionAt(const Vector2& col_position) const;
public:
    MapCollider(float width, float height, Map* map);
    void AdjustTargetPosition(const Vector2& current_pos, Vector2& target_pos) const;
};