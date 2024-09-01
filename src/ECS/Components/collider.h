#pragma once

#include <memory>
#include "../../Math/vector2.h"

class Collider
{
public:
    virtual ~Collider() = default;
    virtual const Vector2& GetPosition() const = 0;
    virtual const float& GetX() const = 0;
    virtual const float& GetY() const = 0;
    
    virtual const float& GetWidth() const = 0;
    virtual const float& GetHeight() const = 0;
    virtual const float& GetHalfWidth() const = 0;
    virtual const float& GetHalfHeight() const = 0;
    
    virtual const bool& GetIsTrigger() const = 0;
    virtual const std::shared_ptr<class Entity>& GetOwner() = 0;
    virtual const std::shared_ptr<class Rigidbody>& GetRigidbody() = 0;

    typedef void (*CollisionCallback)(const std::shared_ptr<Collider>&, void*);
    CollisionCallback on_collision = nullptr;
    void* collision_user_data = nullptr;

    bool is_trigger = false;
    
    virtual void Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, bool one_is_trigger) = 0;
};