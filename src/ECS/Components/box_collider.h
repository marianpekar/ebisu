#pragma once

#include "component.h"
#include <memory>
#include "collider.h"
#include "box_collider.h"

#include "../../Math/vector2.h"

class BoxCollider : public Component, public Collider, public std::enable_shared_from_this<BoxCollider>
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
public:
    BoxCollider(float width, float height, bool is_trigger, const std::shared_ptr<CollisionSolver>& collision_solver);
    void Setup() override;
    void Update(float delta_time) override;
    void Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, bool one_is_trigger) override;
    const bool& GetIsTrigger() const override { return is_trigger; }
    void SetIsTrigger(const bool& value) { is_trigger = value; }
    const float& GetX() const override { return position.x; }
    const float& GetY() const override { return position.y; }
    const float& GetWidth() const override { return width; }
    const float& GetHeight() const override { return height; }
    const float& GetHalfWidth() const override { return half_width; }
    const float& GetHalfHeight() const override { return half_height; }
    const Vector2& GetPosition() const override { return position; }
    const std::shared_ptr<Rigidbody>& GetRigidbody() override { return rigidbody; }
    const std::shared_ptr<Entity>& GetOwner() override { return owner; }
};
