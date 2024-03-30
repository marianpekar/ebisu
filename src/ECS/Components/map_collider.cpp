#include "../../map.h"
#include "map_collider.h"

MapCollider::MapCollider(const float width, const float height,
    const float offset_x, const float offset_y, const std::shared_ptr<Map>& map) :
    width(width), height(height), offset_x(offset_x), offset_y(offset_y), map(map) {}

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
    const Vector2 centered_position = Vector2(col_position.x + offset_x, col_position.y + offset_y);
    return map->HasCollisionAt(centered_position, width, height);
}
