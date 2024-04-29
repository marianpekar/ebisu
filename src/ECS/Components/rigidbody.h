#pragma once

#include <memory>
#include "../../Math/vector2.h"
#include "component.h"

class Rigidbody final : public Component
{
private:
    std::shared_ptr<class Transform> transform;
    std::shared_ptr<class MapCollider> map_collider;
    Vector2 force;
    float mass, drag, acceleration;
public:
    Rigidbody() : force(0,0), mass(1), drag(0.9f), acceleration(1) {}
    void AddForce(const Vector2& force_to_add) { this->force += force_to_add; }
    void ResetForces();
    const Vector2& GetForce() const { return force; }
    void Setup() override;
    void FixedUpdate(float delta_time) override;
    void SetMass(const float& mass);
    void SetDrag(const float& drag);
};