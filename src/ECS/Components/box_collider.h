#pragma once

#include "component.h"
#include <memory>

#include "../../Math/vector2.h"

class BoxCollider final : public Component, public std::enable_shared_from_this<BoxCollider>
{
private:
    Vector2 position;
    float width, height;
    float half_width, half_height;
    bool is_trigger = false;
    std::shared_ptr<class Transform> transform;
    std::shared_ptr<class MapCollider> map_collider;
    std::shared_ptr<class Rigidbody> rigidbody;
    std::shared_ptr<class CollisionSolver> collision_solver;

    typedef void (*CollisionCallback)(const std::shared_ptr<BoxCollider>&, void*);
public:
    CollisionCallback on_collision = nullptr;
    void* collision_user_data = nullptr;
    
    BoxCollider(float width, float height, bool is_trigger, const std::shared_ptr<CollisionSolver>& collision_solver);
    void Setup() override;
    void Update(float delta_time) override;
    void Collide(const std::shared_ptr<BoxCollider>& other, const Vector2& overlap, bool one_is_trigger) const;
    const bool& GetIsTrigger() const { return is_trigger; }
    void SetIsTrigger(const bool& value) { is_trigger = value; }
    const float& GetX() const { return position.x; }
    const float& GetY() const { return position.y; }
    const float& GetWidth() const { return width; }
    const float& GetHeight() const { return height; }
    const float& GetHalfWidth() const { return half_width; }
    const float& GetHalfHeight() const { return half_height; }
    const Vector2& GetPosition() const { return position; }
};