#pragma once

#include <memory>
#include <vector>

struct Rectangle
{
    float x, y;
    float width, height;
    Rectangle(const float x, const float y, const float width, const float height) : 
    x(x), y(y), width(width), height(height) {}
};

class Quadtree
{
private:
    int max_objects = 10;
    int max_levels = 5;
    int level;

    int top_right_quad_index = 0;
    int top_left_quad_index = 1;
    int bottom_left_quad_index = 2;
    int bottom_right_quad_index = 3;
    int no_index = -1;
    
    Rectangle bounds;
    std::vector<std::shared_ptr<class Collider>> objects;
    std::shared_ptr<Quadtree> nodes[4];

    void Split();
    const int& GetIndex(const std::shared_ptr<Collider>& collider) const;
public:
    Quadtree(int level, float x, float y, float width, float height);
    void Clear();
    void Insert(const std::shared_ptr<Collider>& collider);
    std::vector<std::shared_ptr<Collider>> Retrieve(std::vector<std::shared_ptr<Collider>>& result, const std::shared_ptr<Collider>& collider);
};