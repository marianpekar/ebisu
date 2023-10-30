#pragma once

#include <vector>

#include "../../path_node.h"
#include "component.h"

class Agent final : public Component
{
private:
    class Map* map;
    class Transform* transform = nullptr;
    class Rigidbody* rigidbody = nullptr;
    Transform* target_transform = nullptr;
    std::vector<PathNode*> current_path;
    float speed;
    float stopping_distance;
public:
    Agent(Map* map, float speed, float stopping_distance) : map(map), speed(speed), stopping_distance(stopping_distance) {}
    void Setup() override;
    void Update(float delta_time) override;
    void SetTarget(Transform* target);
};
