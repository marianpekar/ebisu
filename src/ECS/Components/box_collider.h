#pragma once

#include "component.h"
#include <memory>
#include "collider.h"
#include "box_collider.h"

#include "../../Math/vector2.h"

class BoxCollider : public Component, public Collider, public std::enable_shared_from_this<BoxCollider>
{
private:
    std::shared_ptr<class Transform> transform;
    std::shared_ptr<class MapCollider> map_collider;
    std::shared_ptr<class Rigidbody> rigidbody;
    std::shared_ptr<class CollisionSolver> collision_solver;
public:
    BoxCollider(float width_val, float height_val, float offset_x_val, float offset_y_val, bool is_trigger_val, const std::shared_ptr<CollisionSolver>& collision_solver);
    void Setup() override;
    void Update(float delta_time) override;
    void Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, bool one_is_trigger) override;

    const std::shared_ptr<Rigidbody>& GetRigidbody() override { return rigidbody; }
    const std::shared_ptr<Entity>& GetOwner() override { return owner; }

#if _DEBUG
    void Render() override;
#endif
};