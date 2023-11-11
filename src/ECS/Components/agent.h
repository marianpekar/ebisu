#pragma once

#include <vector>

#include "component.h"

struct Vector2;

class Agent final : public Component
{
private:
    class Map* map;
    class Transform* transform = nullptr;
    class Rigidbody* rigidbody = nullptr;
    Transform* target_transform = nullptr;
    float speed;
    float stopping_distance;
    std::vector<Vector2> current_path;
    
public:
    Agent(Map* map, float speed, float stopping_distance) : map(map), speed(speed), stopping_distance(stopping_distance) {}
    void Setup() override;
    void Update(float delta_time) override;
    void SetTarget(Transform* target);
#if _DEBUG
    void Render() override;
#endif
};
