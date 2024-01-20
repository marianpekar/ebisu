#include "quadtree.h"
#include "ECS/Components/box_collider.h"

Quadtree::Quadtree(const int level, const float x, const float y, const float width, const float height) :
    level(level), bounds(x, y, width, height), nodes{} {}

void Quadtree::Clear()
{
    objects.clear();

    for (const auto& node : nodes)
    {
        if (node == nullptr)
            continue;

        node->Clear();
    }
}

void Quadtree::Insert(const std::shared_ptr<BoxCollider>& collider)
{
    if (nodes[0] != nullptr) {
        if (const int index = GetIndex(collider); index != -1) 
        {
            nodes[index]->Insert(collider);
            return;
        }
    }
    objects.emplace_back(collider);
    if (static_cast<int>(objects.size()) > max_objects && level < max_levels) 
    {
        if (nodes[0] == nullptr) 
        {
            Split();
        }

        int i = 0;
        while (i < static_cast<int>(objects.size())) 
        {
            if (const int index = GetIndex(objects[i]); index != -1) 
            {
                nodes[index]->Insert(objects[i]);
                objects.erase(objects.begin() + i);
            }
            else 
            {
                i++;
            }
        }
    }
}

const int& Quadtree::GetIndex(const std::shared_ptr<BoxCollider>& collider) const
{
    const float vertical_midpoint = bounds.x + bounds.width / 2;
    const float horizontal_midpoint = bounds.y + bounds.height / 2;

    const bool in_top_quad = collider->GetY() < horizontal_midpoint && collider->GetY() + collider->GetHeight() < horizontal_midpoint;
    const bool in_bottom_quad = collider->GetY() > horizontal_midpoint;

    if (collider->GetX() < vertical_midpoint && collider->GetX() + collider->GetWidth() < vertical_midpoint)
    {
        if (in_top_quad)
            return top_left_quad_index;
        
        if (in_bottom_quad)
            return bottom_left_quad_index;
    }
    else if (collider->GetX() > vertical_midpoint)
    {
        if (in_top_quad)
            return top_right_quad_index;

        if (in_bottom_quad)
            return bottom_right_quad_index;
    }

    return no_index;
}

std::vector<std::shared_ptr<BoxCollider>> Quadtree::Retrieve(std::vector<std::shared_ptr<BoxCollider>>& result, const std::shared_ptr<BoxCollider>& collider)
{
    if (const int index = GetIndex(collider); index != -1 && nodes[0] != nullptr)
    {
        nodes[index]->Retrieve(result, collider);
    }
    result.insert(result.end(), objects.begin(), objects.end());
    return result;
}

void Quadtree::Split()
{
    const float sub_width = bounds.width / 2.0f;
    const float sub_height = bounds.height / 2.0f;
    const float x = bounds.x;
    const float y = bounds.y;

    nodes[0] = new Quadtree(level + 1, x + sub_width, y, sub_width, sub_height);
    nodes[1] = new Quadtree(level + 1, x, y, sub_width, sub_height);
    nodes[2] = new Quadtree(level + 1, x, y + sub_height, sub_width, sub_height);
    nodes[3] = new Quadtree(level + 1, x + sub_width, y + sub_height, sub_width, sub_height);
}