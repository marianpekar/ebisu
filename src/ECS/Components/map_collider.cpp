#include "../../map.h"
#include "map_collider.h"

MapCollider::MapCollider(const float width, const float height, const std::shared_ptr<Map>& map) : width(width), height(height), map(map)
{
    map_tile_size = map->GetTileSize();
}

void MapCollider::AdjustTargetPosition(const Vector2& current_pos, Vector2& target_pos) const
{
    if (HasCollisionAt(Vector2(target_pos.x, current_pos.y)))
    {
        target_pos.x = current_pos.x;
    }

    if (HasCollisionAt(Vector2(current_pos.x, target_pos.y)))
    {
        target_pos.y = current_pos.y;
    }
}

bool MapCollider::HasCollisionAt(const Vector2& col_position) const
{
    const float tl_x = col_position.x;
    const float tl_y = col_position.y;

    const float tr_x = col_position.x + width;
    const float tr_y = col_position.y;

    const float bl_x = col_position.x;
    const float bl_y = col_position.y + height;

    const float br_x = col_position.x + width;
    const float br_y = col_position.y + height;

    const int tl_i = static_cast<int>(tl_x) / map_tile_size;
    const int tl_j = static_cast<int>(tl_y) / map_tile_size;
    
    const int tr_i = static_cast<int>(tr_x) / map_tile_size;
    const int tr_j = static_cast<int>(tr_y) / map_tile_size;
    
    const int bl_i = static_cast<int>(bl_x) / map_tile_size;
    const int bl_j = static_cast<int>(bl_y) / map_tile_size;
    
    const int br_i = static_cast<int>(br_x) / map_tile_size;
    const int br_j = static_cast<int>(br_y) / map_tile_size;

    if (map->GetCollisionAt(tl_i, tl_j) ||
        map->GetCollisionAt(tr_i, tr_j) ||
        map->GetCollisionAt(bl_i, bl_j) ||
        map->GetCollisionAt(br_i, br_j))
    {
        return true;
    }

    return false;
}
