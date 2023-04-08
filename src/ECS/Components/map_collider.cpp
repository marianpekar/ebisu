#include "map_collider.h"
#include "../entity.h"
#include "transform.h"
#include "../../map.h"
#include <iostream>

MapCollider::MapCollider(float width, float height, Map* map) :
	x(0), y(0), width(width), height(height), map(map)
{
	map_size = map->GetMapSize();
	map_tile_size = map->GetTileSize();
}

bool MapCollider::HasCollisionAt(const float& x, const float& y)
{
	float tl_x = x;
	float tl_y = y;

	float tr_x = x + width;
	float tr_y = y;

	float bl_x = x;
	float bl_y = y + height;

	float br_x = x + width;
	float br_y = y + height;

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
