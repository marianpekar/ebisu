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
    return map->HasCollisionAt(col_position, width, height);
}
