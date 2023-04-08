#pragma once

#include "component.h"

class MapCollider : public Component
{
private:
	float x, y, width, height;
	int map_size, map_tile_size;
	class Map* map;
public:
	MapCollider(float width, float height, Map* map);
	bool HasCollisionAt(const float& x, const float& y);
};