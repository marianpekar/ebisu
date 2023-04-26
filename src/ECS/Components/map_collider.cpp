#include <iostream>
#include "../../map.h"
#include "../entity.h"
#include "map_collider.h"
#include "transform.h"

MapCollider::MapCollider(float width, float height, Map* map) :
	position(), width(width), height(height), map(map)
{
	map_size = map->GetMapSize();
	map_tile_size = map->GetTileSize();
}

void MapCollider::AdjustTargetPosition(const Vector2& current_pos, Vector2& target_pos)
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

bool MapCollider::HasCollisionAt(const Vector2& position)
{
	float tl_x = position.x;
	float tl_y = position.y;

	float tr_x = position.x + width;
	float tr_y = position.y;

	float bl_x = position.x;
	float bl_y = position.y + height;

	float br_x = position.x + width;
	float br_y = position.y + height;

	int tl_i = tl_x / map_tile_size;
	int tl_j = tl_y / map_tile_size;

	int tr_i = tr_x / map_tile_size;
	int tr_j = tr_y / map_tile_size;

	int bl_i = bl_x / map_tile_size;
	int bl_j = bl_y / map_tile_size;

	int br_i = br_x / map_tile_size;
	int br_j = br_y / map_tile_size;

	if (map->GetCollisionAt(tl_i, tl_j) ||
		map->GetCollisionAt(tr_i, tr_j) ||
		map->GetCollisionAt(bl_i, bl_j) ||
		map->GetCollisionAt(br_i, br_j))
	{
		return true;
	}

	return false;
}
