#pragma once

#include <memory>
#include "../../Math/vector2.h"

class Collider
{
protected:
    Vector2 position;
    float width, height;
    float half_width, half_height;
    float offset_x = 0;
    float offset_y = 0;

public:
    virtual ~Collider() = default;

    bool GetIsTrigger() const { return is_trigger; }
    void SetIsTrigger(const bool& value) { is_trigger = value; }
    float GetX() const { return position.x; }
    float GetY() const { return position.y; }
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
    float GetHalfWidth() const { return half_width; }
    float GetHalfHeight() const { return half_height; }

    const Vector2& GetPosition() const { return position; }
    void SetPosition(const float x, const float y) { position.x = x; position.y = y; }

    virtual const std::shared_ptr<class Entity>& GetOwner() = 0;
    virtual const std::shared_ptr<class Rigidbody>& GetRigidbody() = 0;

    typedef void (*CollisionCallback)(const std::shared_ptr<Collider>&, void*);
    CollisionCallback on_collision = nullptr;
    void* collision_user_data = nullptr;

    bool is_trigger = false;
    
    virtual void Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, bool one_is_trigger) = 0;
};