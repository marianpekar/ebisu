#include "quadtree.h"
#include "Components/box_collider.h"

Quadtree::Quadtree(int level, float x, float y, float width, float height) :
	level(level), bounds(x, y, width, height), nodes{} {}

void Quadtree::Clear()
{
	objects.clear();

	for (int i = 0; i < QUADTREE_NODES_COUNT; i++)
	{
		if (nodes[i] == nullptr)
			continue;

		nodes[i]->Clear();
		nodes[i] = nullptr;
	}
}

void Quadtree::Insert(BoxCollider* collider)
{
	if (nodes[0] != nullptr) {
		size_t index = GetIndex(collider);
		if (index != -1) 
		{
			nodes[index]->Insert(collider);
			return;
		}
	}
	objects.emplace_back(collider);
	if (objects.size() > max_objects && level < max_levels) 
	{
		if (nodes[0] == nullptr) 
		{
			Split();
		}

		int i = 0;
		while (i < objects.size()) 
		{
			size_t index = GetIndex(objects[i]);
			if (index != -1) 
			{
				nodes[index]->Insert(std::move(objects[i]));
				objects.erase(objects.begin() + i);
			}
			else 
			{
				i++;
			}
		}
	}
}

const size_t& Quadtree::GetIndex(BoxCollider* collider) const
{
	float vertical_midpoint = bounds.x + bounds.width / 2;
	float horizontal_midpoint = bounds.y + bounds.height / 2;

	bool top_quad = collider->GetY() < horizontal_midpoint && collider->GetY() + collider->GetHeight() < horizontal_midpoint;
	bool bottom_quad = collider->GetY() > horizontal_midpoint;

	if (collider->GetX() < vertical_midpoint && collider->GetX() + collider->GetWidth() < vertical_midpoint)
	{
		if (top_quad) 
		{
			return 1;
		}
		else if (bottom_quad) 
		{
			return 2;
		}
	}
	else if (collider->GetX() > vertical_midpoint)
	{
		if (top_quad) 
		{
			return 0;
		}
		else if (bottom_quad) 
		{
			return 3;
		}
	}

	return -1;
}

std::vector<BoxCollider*> Quadtree::Retrieve(std::vector<BoxCollider*>& result, BoxCollider* collider)
{
	int index = GetIndex(collider);
	if (index != -1 && nodes[0] != nullptr)
	{
		nodes[index]->Retrieve(result, collider);
	}
	result.insert(result.end(), objects.begin(), objects.end());
	return result;
}

void Quadtree::Split()
{
	int sub_width = bounds.width / 2;
	int sub_height = bounds.height / 2;
	int x = static_cast<int>(bounds.x);
	int y = static_cast<int>(bounds.y);

	nodes[0] = new Quadtree(level + 1, x + sub_width, y, sub_width, sub_height);
	nodes[1] = new Quadtree(level + 1, x, y, sub_width, sub_height);
	nodes[2] = new Quadtree(level + 1, x, y + sub_height, sub_width, sub_height);
	nodes[3] = new Quadtree(level + 1, x + sub_width, y + sub_height, sub_width, sub_height);
}
