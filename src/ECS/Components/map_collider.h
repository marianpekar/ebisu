#pragma once

#include "../../Math/vector2.h"
#include <memory>
#include "component.h"

class MapCollider final : public Component
{
private:
    Vector2 position; 
    float width, height;
    int map_tile_size;
    std::shared_ptr<class Map> map;
    bool HasCollisionAt(const Vector2& col_position) const;
public:
    MapCollider(float width, float height, const std::shared_ptr<Map>& map);
    void AdjustTargetPosition(const Vector2& current_pos, Vector2& target_pos) const;
};