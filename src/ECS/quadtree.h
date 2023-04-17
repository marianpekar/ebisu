#pragma once

#include <vector>

struct Rectangle
{
	float x, y;
	float width, height;
	Rectangle(float x, float y, float width, float height) : 
	x(x), y(y), width(width), height(height) {}
};

class Quadtree
{
private:
	int max_objects = 10;
	int max_levels = 5;
	int level;
	
	Rectangle bounds;
	std::vector<class BoxCollider*> objects;
	Quadtree* nodes[4];

	void Split();
	const size_t& GetIndex(BoxCollider* collider) const;
public:
	Quadtree(int level, float x, float y, float width, float height);
	void Clear();
	void Insert(BoxCollider* collider);
	std::vector<BoxCollider*> Retrieve(std::vector<BoxCollider*>& result, BoxCollider* collider);
};