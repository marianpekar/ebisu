#pragma once

#include <memory>
#include <vector>

#include "component.h"
#include "../../Math/vector2.h"

class Agent final : public Component
{
private:
    std::shared_ptr<class Map> map;
    std::shared_ptr<class CharacterAnimator> character_animator;
    std::shared_ptr<class Transform> transform;
    std::shared_ptr<class Rigidbody> rigidbody;
    std::shared_ptr<Transform> target_transform;
    float speed;
    float stopping_distance;
    std::vector<Vector2> current_path;
    Vector2 zero_vector = Vector2(0,0);
    Vector2 current_direction; 
public:
    Agent(const std::shared_ptr<Map>& map, float speed, float stopping_distance) : map(map), speed(speed), stopping_distance(stopping_distance) {}
    void Setup() override;
    void Update(float delta_time) override;
    void SetTarget(const std::shared_ptr<Transform>& target);
    void ForceStop();
    
#if _DEBUG
    void Render() override;
#endif
};
