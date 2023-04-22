#pragma once

#include "../../Math/vector2.h"
#include "component.h"


class MapCollider : public Component
{
private:
	Vector2 position; 
	float width, height;
	int map_size, map_tile_size;
	class Map* map;
public:
	MapCollider(float width, float height, Map* map);
	bool HasCollisionAt(const Vector2& position);
};